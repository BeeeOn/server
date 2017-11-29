#pragma once

#include <Poco/AtomicCounter.h>
#include <Poco/Notification.h>
#include <Poco/SharedPtr.h>
#include <Poco/TimedNotificationQueue.h>

#include "dao/FCMTokenDao.h"
#include "fcm/FCMClient.h"
#include "fcm/FCMMessage.h"
#include "fcm/FCMResponse.h"
#include "loop/StoppableRunnable.h"
#include "transaction/Transactional.h"
#include "util/BackOff.h"
#include "util/Loggable.h"

namespace BeeeOn {

class FCMSender :
	public StoppableRunnable,
	public Transactional {
public:
	typedef Poco::SharedPtr<FCMSender> Ptr;

	class MessageHolder : public Poco::Notification {
	public:
		static const std::string TYPE_NAME;

		typedef Poco::AutoPtr<MessageHolder> Ptr;

		MessageHolder(const FCMMessage &message, BackOff::Ptr backOff);

		std::string name() const override;

		FCMMessage &message();
		const FCMMessage &message() const;

		BackOff::Ptr backOff() const;
		Poco::Clock nextTime(const Poco::Timespan &atLeast = 0);

	private:
		FCMMessage m_message;
		BackOff::Ptr m_backOff;
	};

	FCMSender();
	~FCMSender();

	void run() override;
	void stop() override;

	void send(const FCMMessage &message);

	void setServerKey(const std::string &serverKey);
	std::string serverKey() const;
	void setFCMClient(FCMClient::Ptr client);
	void setFCMTokenDao(FCMTokenDao::Ptr dao);
	void setBackOffFactory(BackOffFactory::Ptr factory);

protected:
	void deferSend(const FCMMessage &message, const Poco::Timespan &delay);
	void sendFirst(FCMMessage &message);
	Poco::Timespan doSend(FCMMessage &message);
	void processRecipients(const FCMResponse &response);

private:
	std::string m_serverKey;
	Poco::AtomicCounter m_stop;
	Poco::TimedNotificationQueue m_notificationQueue;
	FCMTokenDao::Ptr m_tokenDao;
	FCMClient::Ptr m_client;
	BackOffFactory::Ptr m_backOffFactory;
};
}
