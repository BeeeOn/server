#include "server/RestHandler.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

RestHandler::RestHandler():
	m_logger(LOGGER_CLASS(this))
{
}