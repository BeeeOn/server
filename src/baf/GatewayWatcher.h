#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "dao/FCMRecipientsDao.h"
#include "fcm/FCMSender.h"
#include "gws/GatewayListener.h"
#include "l10n/Translator.h"
#include "transaction/Transactional.h"

namespace BeeeOn {
namespace Automation {

class GatewayWatcher :
	public GatewayListener,
	public Transactional {
public:
	GatewayWatcher();

	void setRecipientsDao(FCMRecipientsDao::Ptr dao);
	void setSender(FCMSender::Ptr sender);
	void setTranslatorFactory(TranslatorFactory::Ptr factory);

	void onConnected(const GatewayEvent &e) override
	{
		std::vector<FCMMessage> messages;

		BEEEON_TRANSACTION(prepareConnected(e, messages));
		postMessages(messages);
	}

	void onReconnected(const GatewayEvent &e) override
	{
		std::vector<FCMMessage> messages;

		BEEEON_TRANSACTION(prepareReconnected(e, messages));
		postMessages(messages);
	}

	void onDisconnected(const GatewayEvent &e) override
	{
		std::vector<FCMMessage> messages;
		
		BEEEON_TRANSACTION(prepareDisconnected(e, messages));
		postMessages(messages);
	}

protected:
	void prepareConnected(const GatewayEvent &e, std::vector<FCMMessage> &messages);
	void prepareReconnected(const GatewayEvent &e, std::vector<FCMMessage> &messages);
	void prepareDisconnected(const GatewayEvent &e, std::vector<FCMMessage> &messages);

	void findRecipients(
		const GatewayEvent &e,
		std::map<Locale, std::set<FCMTokenID>> &recipients);
	std::vector<FCMMessage> prepareMessages(
		const GatewayEvent &e,
		const std::map<Locale, std::set<FCMTokenID>> &recipients,
		const std::string &titleKey,
		const std::string &bodyKey);
	void postMessages(const std::vector<FCMMessage> &messages);

private:
	FCMRecipientsDao::Ptr m_recipientsDao;
	FCMSender::Ptr m_sender;
	TranslatorFactory::Ptr m_translatorFactory;
};

}
}
