#include <Poco/Logger.h>

#include "util/Loggable.h"
#include "Debug.h"

using namespace Poco;
using namespace BeeeOn;

Loggable::Loggable():
	m_logger(NULL)
{
}

Loggable::~Loggable()
{
}

void Loggable::setupLogger(Logger *logger) const
{
	if (m_logger == NULL) {
		m_logger = logger == NULL?
			&LOGGER_CLASS(this) : logger;
	}
}
