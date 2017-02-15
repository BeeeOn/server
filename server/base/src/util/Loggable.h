#ifndef BEEEON_LOGGABLE_H
#define BEEEON_LOGGABLE_H

#include <typeinfo>
#include <string>

namespace Poco {

class Logger;

}

namespace BeeeOn {

class Loggable {
public:
	Loggable();
	Loggable(const std::type_info &info);
	virtual ~Loggable();

protected:
	void setupLogger(Poco::Logger *logger = 0) const;

	Poco::Logger &logger() const
	{
		setupLogger();
		return *m_logger;
	}

private:
	const std::string m_name;
	mutable Poco::Logger *m_logger;
};

}

#endif
