#include "util/About.h"
#include "di/DIDaemon.h"

using namespace BeeeOn;

int main(int argc, char **argv)
{
	About about;
	about.requirePocoVersion   = 0x01070000;
	about.recommendPocoVersion = 0x01070700;

#ifdef GIT_ID
	about.version = GIT_ID;
#else
	about.version = "out of tree";
#endif

	about.description = "\n"
		"BeeeOn Server runner. This executable is a start-up stub\n"
		"for micro-services that together build up the BeeeOn Server\n"
		"platform. Specify a micro-service configuration to start it.\n"
		"Examples:\n\n"
		"\tbeeeon-server -c xmlui.xml\n"
		"\tbeeeon-server -c restui.xml";

	return DIDaemon::up(argc, argv, about);
}
