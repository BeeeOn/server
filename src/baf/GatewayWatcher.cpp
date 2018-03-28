#include <Poco/Exception.h>

#include "baf/GatewayWatcher.h"
#include "di/Injectable.h"
#include "fcm/FCMMessage.h"

BEEEON_OBJECT_BEGIN(BeeeOn, Automation, GatewayWatcher)
BEEEON_OBJECT_CASTABLE(GatewayListener)
BEEEON_OBJECT_PROPERTY("recipientsDao", &GatewayWatcher::setRecipientsDao)
BEEEON_OBJECT_PROPERTY("sender", &GatewayWatcher::setSender)
BEEEON_OBJECT_PROPERTY("translatorFactory", &GatewayWatcher::setTranslatorFactory)
BEEEON_OBJECT_PROPERTY("transactionManager", &GatewayWatcher::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, Automation, GatewayWatcher)

using namespace std;
using namespace Poco;
using namespace BeeeOn;
using namespace BeeeOn::Automation;

GatewayWatcher::GatewayWatcher()
{
}

void GatewayWatcher::setRecipientsDao(FCMRecipientsDao::Ptr dao)
{
	m_recipientsDao = dao;
}

void GatewayWatcher::setSender(FCMSender::Ptr sender)
{
	m_sender = sender;
}

void GatewayWatcher::setTranslatorFactory(TranslatorFactory::Ptr factory)
{
	m_translatorFactory = factory;
}

void GatewayWatcher::prepareConnected(const GatewayEvent &e, vector<FCMMessage> &messages)
{
	map<Locale, set<FCMTokenID>> recipients;
	findRecipients(e, recipients);

	messages = prepareMessages(e, recipients,
		"baf.gateway_watcher.connected.title",
		"baf.gateway_watcher.connected.body");
}

void GatewayWatcher::prepareReconnected(const GatewayEvent &e, vector<FCMMessage> &messages)
{
	map<Locale, set<FCMTokenID>> recipients;
	findRecipients(e, recipients);

	messages = prepareMessages(e, recipients,
		"baf.gateway_watcher.reconnected.title",
		"baf.gateway_watcher.reconnected.body");
}

void GatewayWatcher::prepareDisconnected(const GatewayEvent &e, vector<FCMMessage> &messages)
{
	map<Locale, set<FCMTokenID>> recipients;
	findRecipients(e, recipients);

	messages = prepareMessages(e, recipients,
		"baf.gateway_watcher.disconnected.title",
		"baf.gateway_watcher.disconnected.body");
}

void GatewayWatcher::findRecipients(
		const GatewayEvent &e,
		map<Locale, set<FCMTokenID>> &recipients)
{
	vector<FCMToken> tokens;
	Gateway gateway(e.gatewayID());

	m_recipientsDao->fetchBy(tokens, gateway);

	// group recipients by locale
	for (const auto &token : tokens) {
		const Locale &locale = token.user().locale();

		const set<FCMTokenID> tokens = {token.id()};

		if (!recipients.emplace(locale, tokens).second)
			recipients[locale].emplace(token.id());
	}
}

vector<FCMMessage> GatewayWatcher::prepareMessages(
		const GatewayEvent &e,
		const map<Locale, set<FCMTokenID>> &recipients,
		const string &titleKey,
		const string &bodyKey)
{
	vector<FCMMessage> messages;

	for (const auto &pair : recipients) {
		Translator::Ptr t = m_translatorFactory->create(pair.first);
		const string &title =
			t->formatSure(titleKey, e.gatewayID().toString());
		const string &body =
			t->formatSure(bodyKey, e.gatewayID().toString(), e.stamp());

		FCMMessage message;
		message.setRecipients(pair.second);
		message.setTitle(title);
		message.setBody(body);

		messages.emplace_back(message);
	}

	return messages;
}

void GatewayWatcher::postMessages(const vector<FCMMessage> &messages)
{
	if (m_sender.isNull())
		return;

	if (logger().debug()) {
		logger().debug(
			"posting " + to_string(messages.size()) + " messages",
			__FILE__, __LINE__);
	}

	for (const auto &message : messages) {
		try {
			m_sender->send(message);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
		}
	}
}
