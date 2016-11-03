#ifndef BEEEON_SQL_DIALECT_H
#define BEEEON_SQL_DIALECT_H

#include <map>
#include <string>

#include "di/InjectorTarget.h"

namespace BeeeOn {

class SQLDialect {
public:
	virtual const std::string name() const = 0;

	/**
	 * Provide a map of specifics of this dialect.
	 */
	virtual void specifics(std::map<std::string, std::string> &data) = 0;

public:
	static const std::string TYPE_UUID;
	static const std::string TYPE_DOUBLE;
	static const std::string TYPE_SMALLINT;
	static const std::string TYPE_INT64;
};

class CustomSQLDialect :
		public SQLDialect,
		public AbstractInjectorTarget {
public:
	CustomSQLDialect();

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string name() const
	{
		return m_name;
	}

	void specifics(std::map<std::string, std::string> &data) override;

	void setTypeUUID(const std::string &value);
	void setDouble(const std::string &value);
	void setSmallInt(const std::string &value);
	void setInt64(const std::string &value);

private:
	std::string m_name;
	std::map<std::string, std::string> m_specifics;
};

}

#endif
