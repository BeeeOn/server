#pragma once

#include <Poco/SharedPtr.h>

#include "dao/DevicePropertyDao.h"
#include "loop/Tool.h"
#include "transaction/Transactional.h"
#include "util/CryptoConfig.h"

namespace BeeeOn {

class DevicePropertyTool : public Tool, public Transactional {
public:
	DevicePropertyTool();

	void setDevicePropertyDao(DevicePropertyDao::Ptr dao);
	void setCryptoConfig(Poco::SharedPtr<CryptoConfig> config);

protected:
	void main(
		ConsoleSession &session,
		const std::vector<std::string> &args) override;

	bool processLine(
		const std::string &line,
		ConsoleSession &session);

	void actionList(
		ConsoleSession &session,
		const std::vector<std::string> &args);
	void actionCreate(
		ConsoleSession &session,
		const std::vector<std::string> &args);
	void actionRemove(
		ConsoleSession &session,
		const std::vector<std::string> &args);

private:
	DevicePropertyDao::Ptr m_propertyDao;
	Poco::SharedPtr<CryptoConfig> m_cryptoConfig;
};

}
