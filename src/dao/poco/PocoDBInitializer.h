#pragma once

#include <string>

#include "dao/poco/PocoScriptExecutor.h"

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoDBInitializer {
public:
	PocoDBInitializer();
	virtual ~PocoDBInitializer();

	void setExecutor(PocoScriptExecutor::Ptr executor);

	virtual void initialize(Poco::Data::Session &session) = 0;

protected:
	unsigned long execute(Poco::Data::Session &session,
			const std::string &script) const;

private:
	PocoScriptExecutor::Ptr m_executor;
};

}
