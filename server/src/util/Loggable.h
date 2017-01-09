#ifndef BEEEON_LOGGABLE_H
#define BEEEON_LOGGABLE_H

namespace Poco {

class Logger;

}

namespace BeeeOn {

class Loggable {
public:
	Loggable();
	virtual ~Loggable();

protected:
	void setupLogger(Poco::Logger *logger = 0) const;

	Poco::Logger &logger() const
	{
		setupLogger();
		return *m_logger;
	}

private:
	mutable Poco::Logger *m_logger;
};

}

#endif
