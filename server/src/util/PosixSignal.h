#ifndef BEEEON_POSIX_SIGNAL_H
#define BEEEON_POSIX_SIGNAL_H

#include <string>

namespace BeeeOn {

class PosixSignal {
private:
	PosixSignal();

public:
	static void send(long pid, const std::string name);

protected:
	static void send(long pid, unsigned int num);
};

}

#endif
