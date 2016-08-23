#ifndef BEEEON_BASE64_H
#define BEEEON_BASE64_H

#include <sstream>
#include <Poco/Base64Encoder.h>
#include <Poco/Base64Decoder.h>
#include <Poco/StreamCopier.h>

namespace BeeeOn {

class Base64 {
public:
	static std::string encode(char *b, size_t blen)
	{
		std::ostringstream sOut;
		Poco::Base64Encoder base64(sOut);
		base64.rdbuf()->setLineLength(0);
		base64.write(b, blen);
		base64.close();

		return sOut.str();
	}

	static size_t decode(const std::string &s, char *b, size_t bcap)
	{
		std::istringstream sIn(s);
		Poco::Base64Decoder base64(sIn);

		for (size_t i = 0; i < bcap; ++i) {
			b[i] = base64.get();

			if (!base64.good() || base64.eof())
				return i;
		}

		return bcap;
	}

	static std::string decode(const std::string &s)
	{
		std::istringstream sIn(s);
		Poco::Base64Decoder base64(sIn);

		std::string result;
		base64 >> result;
		return result;
	}
};

}

#endif
