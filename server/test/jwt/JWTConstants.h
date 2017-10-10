#ifndef BEEEON_JWT_CONSTANTS_H
#define BEEEON_JWT_CONSTANTS_H

#include <string>

namespace BeeeOn {

struct JWTConstants {
	static const std::string HS256_VALID_TOKEN;
	static const std::string HS384_VALID_TOKEN;
	static const std::string HS512_VALID_TOKEN;
	static const std::string NONAL_VALID_TOKEN;
	static const std::string HMAC_SECRET;
};

}

#endif
