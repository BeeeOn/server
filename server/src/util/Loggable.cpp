#include <Poco/Logger.h>

#include "util/Loggable.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Loggable::Loggable():
	m_logger(NULL)
{
}

Loggable::Loggable(const std::type_info &info):
	m_name(BeeeOn::classDemangle(info.name())),
	m_logger(NULL)
{
}

Loggable::~Loggable()
{
}

void Loggable::setupLogger(Logger *logger) const
{
	if (m_logger == NULL) {
		const string name = m_name.empty()?
			classDemangle(typeid(*this).name()) : m_name;

		m_logger = logger == NULL?
			&Poco::Logger::get(name.c_str()) : logger;
	}
}
