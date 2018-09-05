#pragma once

#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/Data/Session.h>

namespace BeeeOn {

/**
 * @brief PocoScriptExecutor represents an interface the provides
 * a way how to execute a given (usually) SQL script.
 */
class PocoScriptExecutor {
public:
	typedef Poco::SharedPtr<PocoScriptExecutor> Ptr;

	virtual ~PocoScriptExecutor();

	/**
	 * @brief Execute the given script via the given session.
	 */
	virtual unsigned long execute(
		Poco::Data::Session &session,
		const std::string &script) const = 0;
};

class PocoDefaultScriptExecutor : public PocoScriptExecutor {
public:
	typedef Poco::SharedPtr<PocoDefaultScriptExecutor> Ptr;

	unsigned long execute(
		Poco::Data::Session &session,
		const std::string &script) const override;
};

}
