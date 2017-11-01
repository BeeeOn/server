#include <unicode/fmtable.h>
#include <unicode/msgfmt.h>

#include <Poco/DateTime.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Timestamp.h>

#include "di/Injectable.h"
#include "l10n/Locale.h"
#include "l10n/IcuLocaleImpl.h"
#include "l10n/IcuTranslator.h"
#include "util/ClassInfo.h"

BEEEON_OBJECT_BEGIN(BeeeOn, IcuTranslatorFactory)
BEEEON_OBJECT_CASTABLE(TranslatorFactory)
BEEEON_OBJECT_TEXT("bundleName", &IcuTranslatorFactory::setBundleName)
BEEEON_OBJECT_END(BeeeOn, IcuTranslatorFactory)

using namespace std;
using namespace Poco;
using namespace Poco::Dynamic;
using namespace BeeeOn;

static void handleWarning(
		Logger &logger,
		const UErrorCode error,
		const string &name,
		const string &locale)
{
	if (logger.debug()) {
		if (error == U_USING_FALLBACK_WARNING) {
			logger.warning(string(u_errorName(error))
				+ " for " + name + "/" + locale,
				__FILE__, __LINE__);
		}
		else if (error == U_USING_DEFAULT_WARNING) {
			logger.warning(string(u_errorName(error))
				+ " for " + name + "/" + locale,
				__FILE__, __LINE__);
		}
	}
}

void IcuTranslator::handleWarning(
		const UErrorCode error,
		const string &name,
		const string &locale) const
{
	::handleWarning(logger(), error, name, locale);
}

static void handleError(
		Logger &logger,
		const UErrorCode error,
		const string &name,
		const string &locale)
{
	if (!U_SUCCESS(error)) {
		string msg = string(u_errorName(error))
			+ " for " + name + "/" + locale;

		switch (error) {
		case U_INVALID_FORMAT_ERROR:
		case U_ILLEGAL_ARGUMENT_ERROR:
		case U_MESSAGE_PARSE_ERROR:
		case U_PARSE_ERROR:
			throw InvalidArgumentException(msg, (int) error);

		case U_INDEX_OUTOFBOUNDS_ERROR:
			throw RangeException(msg, (int) error);

		case U_MISSING_RESOURCE_ERROR:
			throw NotFoundException(msg, (int) error);

		default:
			throw IllegalStateException(msg, (int) error);
		}
	}

	::handleWarning(logger, error, name, locale);
}

void IcuTranslator::handleError(
		const UErrorCode error,
		const string &name,
		const string &locale) const
{
	::handleError(logger(), error, name, locale);
}

string IcuTranslator::typeName(const UResType type) const
{
	switch (type) {
	case URES_NONE:
		return "none";
	case URES_STRING:
		return "string";
	case URES_BINARY:
		return "binary";
	case URES_TABLE:
		return "table";
	case URES_INT:
		return "int";
	case URES_ARRAY:
		return "array";
	case URES_INT_VECTOR:
		return "int-vector";
	default:
		return "<" + to_string((int) type) + ">";
	}
}

static Formattable asFormattable(const Timestamp &ts)
{
	const double ms = ts.epochMicroseconds() / 1000.0;
	return Formattable(ms, Formattable::kIsDate);
}

static Formattable asFormattable(const Var &var)
{
	if (var.isString())
		return Formattable(var.convert<string>().c_str());

	if (var.isBoolean())
		return Formattable(var.convert<bool>());

	if (var.isNumeric()) {
		if (var.isInteger())
			return Formattable(var.convert<long>());

		return Formattable(var.convert<double>());
	}

	if (var.type() == typeid(Timestamp))
		return asFormattable(var.extract<Timestamp>());

	if (var.type() == typeid(DateTime))
		return asFormattable(var.extract<DateTime>().timestamp());

	const ClassInfo type(var.type());
	throw InvalidArgumentException("cannot express " + type.name() + " as formattable");
}

IcuTranslator::IcuTranslator(Poco::SharedPtr<icu::ResourceBundle> bundle):
	m_bundle(bundle)
{
}

string IcuTranslator::applyArgs(
	const icu::ResourceBundle &bundle,
	const vector<Var> &args)
{
	UErrorCode error = U_ZERO_ERROR;
	icu::UnicodeString pattern = bundle.getString(error);
	handleError(error, bundle.getKey(), bundle.getName());

	icu::MessageFormat format(pattern, bundle.getLocale(), error);
	handleError(error, bundle.getKey(), bundle.getName());

	std::vector<icu::Formattable> f;

	for (const auto &arg : args)
		f.push_back(asFormattable(arg));

	icu::UnicodeString result;
	icu::FieldPosition ignore;
	format.format(f.data(), (int) args.size(), result, ignore, error);
	handleError(error, bundle.getKey(), bundle.getName());

	string utf8;
	return result.toUTF8String(utf8);
}

string IcuTranslator::lookupAndApplyArgs(
		const string &originalKey,
		vector<string>::const_iterator current,
		const vector<string>::const_iterator end,
		const ResourceBundle &bundle,
		const vector<Var> &args)
{
	if (current == end)
		return "";

	UErrorCode error = U_ZERO_ERROR;
	icu::ResourceBundle value = bundle.get(current->c_str(), error);

	try {
		handleError(error, originalKey, bundle.getName());
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		return "";
	}

	switch (value.getType()) {
	case URES_STRING:
		return applyArgs(value, args);

	case URES_TABLE:
		return lookupAndApplyArgs(originalKey, ++current, end, value, args);

	default:
		logger().critical(
			"requested resource "
			+ originalKey
			+ " is of type "
			+ typeName(value.getType()),
			__FILE__, __LINE__);

		return "";
	}
}

string IcuTranslator::formatImpl(
		const string &key,
		const vector<Var> &args)
{
	StringTokenizer keyList(key, ".",
		StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);

	const auto &result = lookupAndApplyArgs(
			key, keyList.begin(), keyList.end(), *m_bundle, args);

	return result;
}

static IcuLocaleImpl asIcuLocale(const BeeeOn::Locale &locale)
{
	SharedPtr<IcuLocaleImpl> impl = locale.impl().cast<IcuLocaleImpl>();
	if (!impl.isNull())
		return *impl.get();
	else
		return IcuLocaleImpl::parse(locale.toString());
}

void IcuTranslatorFactory::setBundleName(const string &name)
{
	m_bundleName = name;
}

Translator *IcuTranslatorFactory::create(
		const Locale &loc,
		const string &name)
{
	const IcuLocaleImpl &locale = asIcuLocale(loc);
	const string bundleName = name.empty()? m_bundleName : name;

	UErrorCode error = U_ZERO_ERROR;
	icu::ResourceBundle bundle(bundleName.c_str(), locale.icu(), error);
	handleError(Loggable::forInstance(this), error, bundleName, locale.toString());

	return new IcuTranslator(bundle.clone());
}
