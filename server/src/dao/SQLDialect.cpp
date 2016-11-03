#include "dao/SQLDialect.h"

using namespace std;
using namespace BeeeOn;

BEEEON_OBJECT(CustomSQLDialect, BeeeOn::CustomSQLDialect)

const string SQLDialect::TYPE_UUID = "type_uuid";
const string SQLDialect::TYPE_DOUBLE = "type_double";
const string SQLDialect::TYPE_SMALLINT = "type_smallint";
const string SQLDialect::TYPE_INT64 = "type_int64";

CustomSQLDialect::CustomSQLDialect()
{
	textInjector("name", (TextSetter) &CustomSQLDialect::setName);
	textInjector(TYPE_UUID, (TextSetter) &CustomSQLDialect::setTypeUUID);
	textInjector(TYPE_DOUBLE, (TextSetter) &CustomSQLDialect::setDouble);
	textInjector(TYPE_SMALLINT,
			(TextSetter) &CustomSQLDialect::setSmallInt);
	textInjector(TYPE_INT64, (TextSetter) &CustomSQLDialect::setInt64);
}

void CustomSQLDialect::specifics(map<string, string> &data)
{
	data = m_specifics;
}

void CustomSQLDialect::setTypeUUID(const std::string &value)
{
	m_specifics[TYPE_UUID] = value;
}

void CustomSQLDialect::setDouble(const std::string &value)
{
	m_specifics[TYPE_DOUBLE] = value;
}

void CustomSQLDialect::setSmallInt(const std::string &value)
{
	m_specifics[TYPE_SMALLINT] = value;
}

void CustomSQLDialect::setInt64(const std::string &value)
{
	m_specifics[TYPE_INT64] = value;
}
