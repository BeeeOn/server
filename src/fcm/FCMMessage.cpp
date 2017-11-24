#include <Poco/Exception.h>
#include <Poco/Timestamp.h>

#include "fcm/FCMMessage.h"

using namespace BeeeOn;
using namespace Poco;
using namespace std;

const Timespan FCMMessage::MAX_SUPPORTED_TTL = 28 * Timespan::DAYS;

FCMMessage::FCMMessage():
	m_priority(PRIORITY_NORM),
	m_contentAvailable(true)
{
}

FCMMessage::FCMMessage(const FCMMessage &copy)
{
}

void FCMMessage::addRecipient(const FCMTokenID &to)
{
	m_recipients.insert(to);
}

void FCMMessage::setRecipients(const set<FCMTokenID> &recipients)
{
	m_recipients = recipients;
}

const set<FCMTokenID> &FCMMessage::recipients() const
{
	return m_recipients;
}

void FCMMessage::setTitle(const string &title)
{
	m_title = title;
}

const string &FCMMessage::title() const
{
	return m_title;
}

void FCMMessage::setBody(const string &body)
{
	m_body = body;
}

const string &FCMMessage::body() const
{
	return m_body;
}

void FCMMessage::setTimeToLive(const Timespan &timeToLive)
{
	if (timeToLive < 0)
		throw InvalidArgumentException("TTL must not be negative for FCM message");

	if (timeToLive < MAX_SUPPORTED_TTL)
		m_timeToLive = timeToLive;
	else
		m_timeToLive = MAX_SUPPORTED_TTL;
}

const Timespan &FCMMessage::timeToLive() const
{
	return m_timeToLive;
}

void FCMMessage::setPriority(const Priority &priority)
{
	switch (priority) {
	case PRIORITY_NORM:
	case PRIORITY_HIGH:
		m_priority = priority;
		break;
	default:
		throw InvalidArgumentException("invalid priority given");
	}
}

FCMMessage::Priority FCMMessage::priority() const
{
	return m_priority;
}

void FCMMessage::setContentAvailable(bool contentAvailable)
{
	m_contentAvailable = contentAvailable;
}

bool FCMMessage::contentAvailable() const
{
	return m_contentAvailable;
}
