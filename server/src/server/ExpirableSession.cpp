#include "server/Session.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ExpirableSession::ExpirableSession(
		const UserID &userID,
		const SessionID &sessionID,
		Timespan &timespan):
	Session(userID, sessionID)
{
	// Set expiration time
	m_tstamp += timespan;
}

const Timestamp& ExpirableSession::getExpiration() const
{
	return m_tstamp;
}
