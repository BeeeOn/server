#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/RWLock.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "provider/AuthProvider.h"

namespace BeeeOn {

/**
 * @brief ApiKeyAuthProvider provides authorization of clients identified
 * by a fixed API key. There is a file containing mapping of API keys to
 * users (keyFile) that is used for authorization.
 *
 * The keyFile is loaded via Poco::Util::PropertyFileConfiguration.
 * An example of keyFile contents:
 * <pre>
 * ahqueeshiech4nasevaiY4Vi6shai9eiYeichien = joe.doe@example.org
 * Koofae5xohsh5ilohxoo5ahw3aiy4LuofeeBuef0 = john.smith@example.org
 * xahneilee5ewiozech5pheiBoaJathei6phai8ie = michael.jackson@example.org
 * tah2eirieghohwie2Ileej1ahshoraiRohsephah = freddy.mercury@example.org
 * tah2eirieghohwie2Ileej1ahshoraiRohsephah.first_name = Freddy
 * tah2eirieghohwie2Ileej1ahshoraiRohsephah.last_name = Mercury
 * tah2eirieghohwie2Ileej1ahshoraiRohsephah.locale = en_GB
 * </pre>
 *
 * It is possible to specify only the mapping as <KEY> = <EMAIL>. However,
 * first_name, last_name and locale can be optionally given for each <KEY>.
 *
 * The keyFile is reloaded when changed. The reload is performed on demand
 * but only once per the reloadPeriod (unless it is zero).
 */
class ApiKeyAuthProvider : public AbstractAuthProvider {
public:
	ApiKeyAuthProvider();

	void setKeyFile(const std::string &path);
	void setReloadPeriod(const Poco::Timespan &period);

	bool authorize(const Credentials &cred, AuthResult &result) override;

protected:
	void reloadKeys(bool force = false);
	bool shouldReload(const Poco::File &file) const;

private:
	Poco::Path m_keysFile;
	Poco::Timespan m_reloadPeriod;
	Poco::Timestamp m_lastReload;
	size_t m_lastSize;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_keys;
	Poco::RWLock m_lock;
};

}
