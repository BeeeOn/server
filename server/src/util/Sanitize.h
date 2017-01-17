#ifndef BEEEON_SANITIZE_H
#define BEEEON_SANITIZE_H

#include <string>

namespace BeeeOn {

class Sanitize {
public:
	enum {
		/**
		 * When we have *really* no idea about maximal length.
		 */
		SIZE_LIMIT_NONE   = 0,
		/**
		 * E-mail addresses should not be longer.
		 */
		SIZE_LIMIT_EMAIL  = 512,
	};

	/**
	 * Sanitize encoding on the given input bytes. The bytes are converted
	 * to UTF-8 and checked for invalid characters. The default input
	 * encoding is UTF-8, however, it can be changed. The output encoding
	 * is always UTF-8.
	 *
	 * The sizeLimit is intended to avoid too long inputs. Inputs of size
	 * longer then the sizeLimit bytes are never processed. Note that the
	 * input can contain less characters then bytes (UTF-8). If the sizeLimit
	 * is given as SIZE_LIMIT_NONE then no size check is performed.
	 */
	static std::string encoding(const std::string &bytes,
			const unsigned long sizeLimit = SIZE_LIMIT_NONE,
			const std::string &inputEncoding = "UTF-8");

	/**
	 * Sanitize e-mail address. Convert to UTF-8 and check length and format.
	 */
	static std::string email(const std::string &bytes,
			const unsigned long sizeLimit = SIZE_LIMIT_EMAIL,
			const std::string &inputEncoding = "UTF-8");
};

}

#endif
