#ifndef BEEEON_RESTUI_SERIALIZING_H
#define BEEEON_RESTUI_SERIALIZING_H

#include <vector>

namespace Poco {
namespace JSON {

class PrintHandler;

}
}

namespace BeeeOn {

class Gateway;

namespace RestUI {

void serialize(Poco::JSON::PrintHandler &output, const Gateway &gateway);
void serialize(Poco::JSON::PrintHandler &output,
		const std::vector<Gateway> &gateways);

}

}

#endif
