#include "fcm/FCMResponse.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

const Timespan FCMResponse::UNKNOWN_DELAY = -1;

FCMResponse::FCMResponse():
	m_retryAfter(UNKNOWN_DELAY)
{
}

void FCMResponse::setRetryAfter(const Timespan &delay)
{
	m_retryAfter = delay;
}

const Timespan &FCMResponse::retryAfter() const
{
	return m_retryAfter;
}

void FCMResponse::setUnderliveredRecipients(const set<FCMTokenID> &recipients)
{
	m_undelivered = recipients;
}

const set<FCMTokenID> &FCMResponse::undeliveredRecipients() const
{
	return m_undelivered;
}

void FCMResponse::setFailingRecipients(const set<FCMTokenID> &failing)
{
	m_failing = failing;
}

const set<FCMTokenID> &FCMResponse::failingRecipients() const
{
	return m_failing;
}

void FCMResponse::setRenewedRecipients(const map<FCMTokenID, FCMTokenID> &recipients)
{
	m_renewed = recipients;
}

const map<FCMTokenID, FCMTokenID> &FCMResponse::renewedRecipients() const
{
	return m_renewed;
}
