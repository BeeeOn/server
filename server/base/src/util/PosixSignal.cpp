#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstring>

#include <Poco/Exception.h>

#include "util/PosixSignal.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

PosixSignal::PosixSignal()
{
}

void PosixSignal::send(long pid, unsigned int num)
{
	pid_t epid = (pid_t) pid;

	if (epid <= 0)
		throw InvalidArgumentException("invalid pid given");

	if (kill(epid, num) < 0)
		throw IllegalStateException(string("failed to send signal ") + strerror(errno));
}

void PosixSignal::send(long pid, const string name)
{
	unsigned int num = 0;

	if (name == "SIGTERM")
		num = SIGTERM;
	else if (name == "SIGINT")
		num = SIGINT;
	else if (name == "SIGKILL")
		num = SIGKILL;
	else if (name == "SIGUSR1")
		num = SIGUSR1;
	else if (name == "SIGUSR2")
		num = SIGUSR2;
	else if (name == "SIGCONT")
		num = SIGCONT;
	else if (name == "SIGHUP")
		num = SIGHUP;
	else
		throw InvalidArgumentException("unrecognized signal name");

	send(pid, num);
}
