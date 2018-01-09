#include <sstream>

#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Exception.h>
#include <Poco/NumberFormatter.h>
#include <Poco/String.h>
#include <Poco/Timespan.h>
#include <Poco/UnicodeConverter.h>

#include <Poco/Data/AbstractBinder.h>
#include <Poco/Data/AbstractBinding.h>
#include <Poco/Data/StatementImpl.h>

#ifdef POCO_ODBC
#include <Poco/Data/ODBC/ODBCException.h>
#endif

#include "dao/QueryLoader.h"
#include "dao/Query.h"
#include "dao/poco/PocoTransactionImpl.h"
#include "dao/poco/PocoAbstractDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "transaction/TransactionManager.h"
#include "util/Backtrace.h"
#include "util/ClassInfo.h"
#include "util/PocoStatementIntruder.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

PocoAbstractDao::PocoAbstractDao():
	m_manager(NULL),
	m_loader(NULL)
{
}

PocoAbstractDao::~PocoAbstractDao()
{
}

void PocoAbstractDao::setDaoManager(PocoDaoManager *manager)
{
	m_manager = manager;
}

void PocoAbstractDao::setTransactionManager(TransactionManager::Ptr manager)
{
	m_transactionManager = manager;
}

void PocoAbstractDao::setQueryLoader(QueryLoader::Ptr loader)
{
	m_loader = loader;
}

PocoDaoManager &PocoAbstractDao::manager()
{
	if (m_manager)
		return *m_manager;

	throw IllegalStateException("missing manager");
}

Poco::Data::Session PocoAbstractDao::session(bool transact)
{
	Session session(openSession(transact));
	manager().customizeSession(session);

	return session;
}

Poco::Data::Session PocoAbstractDao::openSession(bool transact)
{
	Transaction *t = m_transactionManager->current();

	if (t == NULL) {
		if (transact && logger().warning()) {
			logger().warning("session is out of transaction management",
					__FILE__, __LINE__);
		}

		return manager().pool().get();
	}
	else {
		try {
			return t->impl<PocoTransactionImpl>().session();
		}
		catch (std::bad_cast &e) {
			throw IllegalStateException(
				"incompatible transaction, expected BeeeOn::PocoTransactionImpl");
		}
	}
}

void PocoAbstractDao::registerQuery(Query &query)
{
	m_queries.push_back(&query);
}

namespace BeeeOn {

class IntrudingBinder : public Poco::Data::AbstractBinder {
public:
	typedef SharedPtr<IntrudingBinder> Ptr;

	enum {
		STRING_LENGTH_LIMIT = 64,
		BLOB_LENGTH_LIMIT = 32,
	};

#define GENERATE_BIND(type, method) \
	void bind(size_t, const type &v, Direction) override \
	{                                                    \
		m_value = method(v);                         \
	}                                                    \
	void bind(size_t, const std::vector<type> &v,        \
			Direction) override                  \
	{                                                    \
		m_value = "[";                               \
		for (const auto &one : v)                    \
			m_value += method(one) + " ";        \
		*m_value.rbegin() = ']';                     \
	}                                                    \
	void bind(size_t, const std::deque<type> &v,         \
			Direction) override                  \
	{                                                    \
		m_value = "[";                               \
		for (const auto &one : v)                    \
			m_value += method(one) + " ";        \
		*m_value.rbegin() = ']';                     \
	}                                                    \
	void bind(size_t, const std::list<type> &v,          \
			Direction) override                  \
	{                                                    \
		m_value = "[";                               \
		for (const auto &one : v)                    \
			m_value += method(one) + " ";        \
		*m_value.rbegin() = ']';                     \
	}

	GENERATE_BIND(Int8, to_string)
	GENERATE_BIND(UInt8, to_string)
	GENERATE_BIND(Int16, to_string)
	GENERATE_BIND(UInt16, to_string)
	GENERATE_BIND(Int32, to_string)
	GENERATE_BIND(UInt32, to_string)
	GENERATE_BIND(Int64, to_string)
	GENERATE_BIND(UInt64, to_string)
	GENERATE_BIND(bool, to_string)
	GENERATE_BIND(float, to_string)
	GENERATE_BIND(double, to_string)
	GENERATE_BIND(char, to_string)
	GENERATE_BIND(string, makeSnippet)
	GENERATE_BIND(UTF16String, toUTF8)
	GENERATE_BIND(BLOB, fromBLOB)
	GENERATE_BIND(CLOB, fromCLOB)
	GENERATE_BIND(DateTime, fromDateTime)
	GENERATE_BIND(Date, fromDate)
	GENERATE_BIND(Time, fromTime)
	GENERATE_BIND(NullData, fromNull)

	void bind(size_t, const char * const &v, Direction)
	{
		const size_t len = strlen(v);

		if (len > STRING_LENGTH_LIMIT)
			m_value = makeSnippet({v, STRING_LENGTH_LIMIT + 1});
		else
			m_value = makeSnippet(v);
	}

	void bind(size_t, const Any &v, Direction)
	{
		m_value = "Any(";
		m_value += v.type().name();
		m_value += ")";
	}

	void bind(size_t, const Dynamic::Var &v, Direction)
	{
		if (v.isString())
			m_value = makeSnippet(v.extract<string>());
		else if (v.isNumeric() || v.isBoolean())
			m_value = v.toString();
		else if (v.isDeque())
			m_value = "deque (" + to_string(v.size()) + ")";
		else if (v.isList())
			m_value = "list (" + to_string(v.size()) + ")";
		else if (v.isArray())
			m_value = "array (" + to_string(v.size()) + ")";
		else if (v.isStruct())
			m_value = "struct (" + to_string(v.size()) + ")";
		else if (v.isEmpty())
			m_value = "NULL";
		else
			m_value = v.toString();
	}

	string value() const
	{
		return m_value;
	}

	static string toUTF8(const UTF16String &s)
	{
		string result;
		UnicodeConverter::convert(s, result);
		return makeSnippet(result);
	}

	static string fromBLOB(const BLOB &blob)
	{
		string buffer;
		size_t i = 0;

		for (const auto b : blob) {
			if (i >= BLOB_LENGTH_LIMIT) {
				buffer += "... ";
				break;
			}

			NumberFormatter::appendHex(buffer, (unsigned int) b, 2);

			buffer += " ";
			i += 1;
		}

		return buffer + "(" + to_string(blob.size()) + ")";
	}

	static string fromCLOB(const CLOB &clob)
	{
		string buffer;

		for (const auto c : clob) {
			buffer += c;

			if (buffer.size() > STRING_LENGTH_LIMIT)
				break;
		}

		return makeSnippet(buffer, clob.size());
	}

	static string fromDateTime(const DateTime &dt)
	{
		return DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT);
	}

	static string fromDate(const Date &date)
	{
		DateTime dt(date.year(), date.month(), date.day());
		return DateTimeFormatter::format(dt, "%Y-%m-%d");
	}

	static string fromTime(const Time &time)
	{
		return DateTimeFormatter::format(
			Timespan(time.hour(), time.minute(), time.second(), 0, 0));
	}

	static string fromNull(const NullData &null)
	{
		return "NULL";
	}

	static string makeSnippet(const string &s, const size_t size = 0)
	{
		const size_t effective = size == 0? s.size() : size;
		const string len = "(" + to_string(effective) + ")";

		if (effective > STRING_LENGTH_LIMIT)
			return s.substr(0, STRING_LENGTH_LIMIT) + "... " + len;

		return s + " " + len;
	}

private:
	std::string m_value;
};

}

static void formatStatementInfo(
		ostringstream &buffer,
		const Statement &stmt)
{
	buffer << stmt.toString() << endl;

	PocoStatementIntruder intruder(stmt);

	for (auto &binding : intruder.impl().bindings()) {
		if (binding->getDirection() == AbstractBinder::PD_OUT)
			continue;

		buffer << "  ";
		buffer << binding->name();

		binding->reset();
		IntrudingBinder::Ptr binder = new IntrudingBinder;
		binding->setBinder(binder);
		binding->bind(0);

		buffer << "=";
		buffer << binder->value();
		buffer << endl;
	}
}

static void finishHandleException(const string &message, const Poco::Exception e)
{
	Data::DataException exception(message, e);
	exception.rethrow();
}

#ifdef POCO_ODBC
static void handleException(
		const Statement &stmt,
		const ODBC::StatementException &e)
{
	ostringstream buffer;

	buffer << "ODBC StatementException" << endl;

	const auto &diag = e.diagnostics();

	for (int i = 0; i < diag.count(); ++i) {
		buffer << "[" << i << "] ";
		buffer << "SQL state: " << diag.sqlState(i);
		buffer << ", ";
		buffer << "Native error: " << diag.nativeError(i);
		buffer << endl;
		buffer << trimRight(diag.message(i)) << endl;
	}

	formatStatementInfo(buffer, stmt);
	finishHandleException(buffer.str(), e);
}
#endif

static void handleException(
		const Statement &stmt,
		const Exception &e)
{
	ostringstream buffer;

	buffer << trimRight(e.displayText()) << endl;

	formatStatementInfo(buffer, stmt);
	finishHandleException(buffer.str(), e);
}

size_t PocoAbstractDao::execute(Statement &sql)
{
	size_t result;

	try {
		result = sql.execute();

		if (logger().trace()) {
			ostringstream buffer;

			buffer << "[result: " << result << "] ";
			formatStatementInfo(buffer, sql);

			logger().trace(buffer.str(), __FILE__, __LINE__);
		}
		else if (logger().debug()) {
			string prefix("[result: ");
			prefix += to_string(result) + "] ";
			logger().debug(prefix + sql.toString(),
					__FILE__, __LINE__);
		}

		return result;
	}
#ifdef POCO_ODBC
	catch (const ODBC::StatementException &e) {
		handleException(sql, e);
		throw;
	}
#endif
	catch (const Exception &e) {
		handleException(sql, e);
		throw;
	}
}

RecordSet PocoAbstractDao::executeSelect(Statement &sql)
{
	execute(sql);
	RecordSet result(sql);
	return result;
}

void PocoAbstractDao::throwMissingId(const type_info &t)
{
	Backtrace trace;
	logger().critical(trace.toString());

	throw InvalidArgumentException("missing id for "
			+ ClassInfo(t).name());
}

double PocoAbstractDao::nanWhenEmpty(const Poco::Dynamic::Var &v)
{
	return v.isEmpty()? (double) NAN : v.convert<double>();
}

std::string PocoAbstractDao::emptyWhenNull(const Poco::Dynamic::Var &v)
{
	return v.isEmpty()? "" : v.toString();
}

bool PocoAbstractDao::hasColumn(const RecordSet &result, const std::string &name)
{
	const size_t count = result.columnCount();

	for (size_t i = 0; i < count; ++i) {
		if (result.columnName(i) == name)
			return true;
	}

	return false;
}

bool PocoAbstractDao::hasColumn(const Row &result, const std::string &name)
{
	const Row::NameVecPtr columns = result.names();

	for (auto column : *columns) {
		if (name == column)
			return true;
	}

	return false;
}

void PocoAbstractDao::loadQueries()
{
	for (auto query : m_queries) {
		logger().debug("loading query " + query->key(),
				__FILE__, __LINE__);

		query->load(*m_loader);
	}
}
