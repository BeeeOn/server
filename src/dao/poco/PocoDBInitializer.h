#ifndef BEEEON_POCO_DB_INITIALIZER_H
#define BEEEON_POCO_DB_INITIALIZER_H

#include <string>

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoDBInitializer {
public:
	virtual void initialize(Poco::Data::Session &session) = 0;

protected:
	unsigned long execute(Poco::Data::Session &session,
			const std::string &script) const;
};

}

#endif
