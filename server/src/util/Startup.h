#ifndef BEEEON_STARTUP_H
#define BEEEON_STARTUP_H

#include <Poco/Util/Application.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/File.h>

namespace BeeeOn {

class ServerStartup : public Poco::Util::ServerApplication {
public:
	ServerStartup(const std::string appGroup,
			const std::string appName,
			unsigned int port):
		m_appGroup(appGroup),
		m_appName(appName),
		m_printHelp(false),
		m_serverPort(port),
		m_notifyPid(-1)
	{
	}

protected:
	/**
	 * Extend log-level parsing by shortcuts: info, warn, err, cirt.
	 */
	int parseLogLevel(const std::string &level);

	/**
	 * Override a configuration option given as <key>=<value>.
	 */
	void overrideConfig(const std::string text);

	void handleOption(const std::string &name, const std::string &value);

	void loadAllConfiguration();
	std::string parentPath(const Poco::File &file);
	bool readConfiguration(const Poco::File &file);
	void findAndLoadConfig();
	void findAndLoadLogging();
	void findAndLoadServices();
	void initialize(Poco::Util::Application &app) override;
	void defineOptions(Poco::Util::OptionSet &options) override;
	int printHelp();
	void notifyStarted();
	int main(const std::vector <std::string> &args) override;

	virtual int execute() = 0;

	virtual std::string defaultLoggingFile() const;
	virtual std::string defaultConfigFile() const;
	virtual std::string defaultServicesFile() const;

protected:
	std::string m_appGroup;
	std::string m_appName;
	bool m_printHelp;
	unsigned int m_serverPort;
	std::string m_userLogging;
	std::string m_userConfig;
	std::string m_userServices;
	long m_notifyPid;
};

int generic_main(int argc, char **argv, Poco::Util::ServerApplication &app);

}

#endif
