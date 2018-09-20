#pragma once

#include <iosfwd>

#include <Poco/Data/Statement.h>

namespace BeeeOn {

/**
 * @brief PocoStatementInfo can internally inspect the given statement
 * and it provides information about it without anybody being aware
 * of how is the inspection implemented.
 */
class PocoStatementInfo {
public:
	PocoStatementInfo(const Poco::Data::Statement &statement);

	/**
	 * @brief Dump information about the statement (its bindings)
	 * into the given ostream.
	 */
	void dump(std::ostream &out) const;

private:
	const Poco::Data::Statement &m_statement;
};

}
