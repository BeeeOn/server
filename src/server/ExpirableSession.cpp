#include "server/Session.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ExpirableSession::ExpirableSession(
		const UserID &userID,
		const SessionID &sessionID,
		Timespan &timespan):
	Session(userID, sessionID, timespan)
{
}
