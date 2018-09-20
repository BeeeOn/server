#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/NumberFormatter.h>
#include <Poco/SharedPtr.h>
#include <Poco/String.h>
#include <Poco/Timespan.h>
#include <Poco/UnicodeConverter.h>

#include <Poco/Data/AbstractBinder.h>
#include <Poco/Data/AbstractBinding.h>
#include <Poco/Data/StatementImpl.h>

#include "dao/poco/PocoStatementInfo.h"
#include "util/PocoStatementIntruder.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

PocoStatementInfo::PocoStatementInfo(
		const Statement &statement):
	m_statement(statement)
{
}

namespace BeeeOn {

class IntrudingBinder : public AbstractBinder {
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
	string m_value;
};

}

void PocoStatementInfo::dump(ostream &out) const
{
	out << m_statement.toString() << endl;

	PocoStatementIntruder intruder(m_statement);

	for (auto &binding : intruder.impl().bindings()) {
		if (binding->getDirection() == AbstractBinder::PD_OUT)
			continue;

		out << "  ";
		out << binding->name();

		binding->reset();
		IntrudingBinder::Ptr binder = new IntrudingBinder;
		binding->setBinder(binder);
		binding->bind(0);

		out << "=";
		out << binder->value();
		out << endl;
	}
}
