#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/TimedNotificationQueue.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
#include <Poco/Notification.h>

#include <Poco/Net/SocketNotification.h>

#include "di/Injectable.h"
#include "fcm/FCMSender.h"
#include "model/FCMToken.h"
#include "util/ExponentialBackOff.h"

using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;
using namespace std;

BEEEON_OBJECT_BEGIN(BeeeOn, FCMSender)
BEEEON_OBJECT_CASTABLE(StoppableRunnable)
BEEEON_OBJECT_PROPERTY("serverKey", &FCMSender::setServerKey)
BEEEON_OBJECT_PROPERTY("fcmClient", &FCMSender::setFCMClient)
BEEEON_OBJECT_PROPERTY("fcmTokenDao", &FCMSender::setFCMTokenDao)
BEEEON_OBJECT_PROPERTY("backOffFactory", &FCMSender::setBackOffFactory)
BEEEON_OBJECT_PROPERTY("transactionManager", &FCMSender::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, FCMSender)

FCMSender::FCMSender():
	m_stop(false)
{
}

FCMSender::~FCMSender()
{
}

void FCMSender::setServerKey(const string &serverKey)
{
	m_serverKey = serverKey;
}

string FCMSender::serverKey() const
{
	return m_serverKey;
}

void FCMSender::setFCMTokenDao(FCMTokenDao::Ptr dao)
{
	m_tokenDao = dao;
}

void FCMSender::setBackOffFactory(BackOffFactory::Ptr factory)
{
	m_backOffFactory = factory;
}

void FCMSender::send(const FCMMessage &message)
{
	FCMMessage copy(message);
	sendFirst(copy);
}

void FCMSender::sendFirst(FCMMessage &message)
{
	try {
		const Timespan &retryAfter = doSend(message);

		if (!message.recipients().empty())
			deferSend(message, retryAfter);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		e.rethrow();
	}
}

void FCMSender::processRecipients(const FCMResponse &response)
{
	for (const auto &one : response.failingRecipients()) {
		FCMToken token = one;
		m_tokenDao->remove(token);
	}

	const auto &renewed = response.renewedRecipients();
	for (const auto &pair : renewed) {
		FCMToken original = pair.first;
		FCMToken refresh = pair.second;

		m_tokenDao->replace(original, refresh);
	}
}

Timespan FCMSender::doSend(FCMMessage &message)
{
	const FCMResponse &response = m_client->sendMessage(message, m_serverKey);

	BEEEON_TRANSACTION(processRecipients(response));

	const auto &recipients = response.undeliveredRecipients();
	message.setRecipients(recipients);

	return response.retryAfter();
}

void FCMSender::deferSend(const FCMMessage &message, const Timespan &)
{
	BackOff::Ptr backOff = m_backOffFactory->create();

	// TODO delay is ignored in this implementation because of lacking of
	// a proper API, we need something like:
	//	backOff->setInitialInterval(delay.totalMicroseconds());

	MessageHolder::Ptr holder = new MessageHolder(message, backOff);
	const Clock nextTime = holder->nextTime();

	if (nextTime == 0) {
		logger().warning("dropping message due to back-off policy",
				__FILE__, __LINE__);
	}
	else {
		m_notificationQueue.enqueueNotification(holder, nextTime);
	}
}

void FCMSender::setFCMClient(FCMClient::Ptr client)
{
	m_client = client;
}

void FCMSender::run()
{
	while (!m_stop) {
		Notification::Ptr n = m_notificationQueue.waitDequeueNotification();
		if (n->name() != MessageHolder::TYPE_NAME)
			break;

		MessageHolder::Ptr holder = n.cast<MessageHolder>();
		Timespan delay;

		try {
			delay = doSend(holder->message());
		}
		catch (const IllegalStateException &e) {
			logger().log(e, __FILE__, __LINE__);
			continue;
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			// defer sending
		}

		if (!holder->message().recipients().empty()) {
			const Clock nextTime = holder->nextTime();

			if (nextTime == 0) {
				logger().warning(
					"dropping message due to repeated delivery failures",
					__FILE__, __LINE__);
			}
			else {
				m_notificationQueue.enqueueNotification(holder, nextTime);
			}
		}
	}

	// clean up the queue
	size_t count = 0;
	Notification::Ptr n;

	while ((n = m_notificationQueue.dequeueNotification())) {
		if (!n.cast<ShutdownNotification>().isNull())
			continue;

		if (logger().trace()) {
			MessageHolder::Ptr holder = n.cast<MessageHolder>();
			if (!holder.isNull()) {
				logger().trace("dropping message '"
					+ holder->message().title() + "'",
					__FILE__, __LINE__);
			}
			else {
				logger().trace("dropping unknown notification '"
					+ n->name() + "'",
					__FILE__, __LINE__);
			}
		}

		count += 1;
	}

	if (count > 0) {
		logger().warning(
			"dropped " + to_string(count) + " deferred messages",
			__FILE__, __LINE__);
	}
}

void FCMSender::stop()
{
	m_stop = true;

	const Clock now;
	m_notificationQueue.enqueueNotification(
			new ShutdownNotification(NULL), now);
}

const string FCMSender::MessageHolder::TYPE_NAME = "fcm-message-holder";

FCMSender::MessageHolder::MessageHolder(
		const FCMMessage &message, BackOff::Ptr backOff):
	m_message(message),
	m_backOff(backOff)
{
}

string FCMSender::MessageHolder::name() const
{
	return TYPE_NAME;
}

FCMMessage &FCMSender::MessageHolder::message()
{
	return m_message;
}

const FCMMessage &FCMSender::MessageHolder::message() const
{
	return m_message;
}

BackOff::Ptr FCMSender::MessageHolder::backOff() const
{
	return m_backOff;
}

Clock FCMSender::MessageHolder::nextTime(const Timespan &atLeast)
{
	const Clock now;
	const Timespan &delay = m_backOff->next();

	if (delay == BackOff::STOP)
		return 0;

	if (delay > atLeast)
		return now + delay.totalMicroseconds();
	else
		return now + atLeast.totalMicroseconds();
}
