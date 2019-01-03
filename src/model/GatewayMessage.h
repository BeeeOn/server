#pragma once

#include <string>

#include <Poco/Timestamp.h>
#include <Poco/JSON/Object.h>

#include "l10n/Translator.h"

#include "model/Entity.h"
#include "model/Gateway.h"
#include "model/GlobalID.h"
#include "model/Severity.h"

namespace BeeeOn {

/**
 * @brief GatewayMessage represents a message related somehow
 * to a gateway. The message informs its users about some issues
 * or events occuring at the gateway or when communicating with
 * the gateway.
 *
 * The message itself is a compound of:
 *
 * - severity - error, warn, info
 * - key - key to lookup the displayable parametrizable text
 * - context - JSON object with parameters of the displayable text
 *
 * The messages are thus defined by the server as a fixed list
 * in some resource file.
 */
class GatewayMessage : public Entity<GlobalID> {
public:
	GatewayMessage();
	GatewayMessage(const ID &id);

	void setGateway(const Gateway &gateway);
	const Gateway &gateway() const;

	void setAt(const Poco::Timestamp &at);
	Poco::Timestamp at() const;

	void setSeverity(const Severity &severity);
	Severity severity() const;

	void setKey(const std::string &key);
	std::string key() const;

	void setContext(const Poco::JSON::Object::Ptr context);
	Poco::JSON::Object::Ptr context() const;

	/**
	 * @brief Translate the message via the given translator. The context
	 * is used for this purpse. The context entries are sorted by key names
	 * and the sorted values are used for the actual translation.
	 */
	std::string translate(Translator& translator) const;

protected:
	void validateContext(const Poco::JSON::Object::Ptr context) const;

private:
	Gateway m_gateway;
	Poco::Timestamp m_at;
	Severity m_severity;
	std::string m_key;
	Poco::JSON::Object::Ptr m_context;
};

typedef GatewayMessage::ID GatewayMessageID;

}
