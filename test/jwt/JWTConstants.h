#pragma once

#include <string>

namespace BeeeOn {

struct JWTConstants {
	static const std::string HS256_VALID_TOKEN;
	static const std::string HS384_VALID_TOKEN;
	static const std::string HS512_VALID_TOKEN;
	static const std::string RS256_VALID_TOKEN;
	static const std::string RS384_VALID_TOKEN;
	static const std::string RS512_VALID_TOKEN;
	static const std::string NONAL_VALID_TOKEN;
	static const std::string HMAC_SECRET;
	static const std::string RSA_SECRET;
};

}
