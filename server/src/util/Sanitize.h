#ifndef BEEEON_SANITIZE_H
#define BEEEON_SANITIZE_H

#include <string>
#include <Poco/URI.h>

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
		/**
		 * Common text (single field) to be inserted into an XML.
		 * We can consider longer texts (like for <textarea>).
		 */
		SIZE_LIMIT_XML    = 4096,
		/**
		 * Common names are pretty short.
		 */
		SIZE_LIMIT_COMMON = 1024,
		/**
		 * Be strict, we do not expect this kind of data to be
		 * longer then necessary. Usually max 32 characters.
		 */
		SIZE_LIMIT_STRICT = 128,
		/**
		 * It is difficult to limit URIs. The limit should be
		 * quite high.
		 */
		SIZE_LIMIT_URI    = 1024,
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

	/**
	 * Sanitize XML content. Convert to UTF-8 and convert non-standard characters
	 * to entities. Useful especially for output sanitization.
	 */
	static std::string xml(const std::string &bytes,
			const unsigned long sizeLimit = SIZE_LIMIT_XML,
			const std::string &inputEncoding = "UTF-8");

	/**
	 * Sanitization of common text input. It is suitable for non-rich text
	 * with alpha-numeric characters, spaces and a small subset of punctation
	 * symbols.
	 */
	static std::string common(const std::string &bytes,
			const unsigned long sizeLimit = SIZE_LIMIT_COMMON,
			const std::string &inputEncoding = "UTF-8");

	/**
	 * Sanitize the text input. Convert the contents to UTF-8
	 * and allow only basic symbols like: [-a-zA-Z0-9 \t]*.
	 */
	static std::string strict(const std::string &bytes,
			const unsigned long sizeLimit = SIZE_LIMIT_STRICT,
			const std::string &inputEncoding = "UTF-8");

	/**
	 * Sanitize URI. It is URL-decoded and checked to be UTF-8 valid.
	 * Finally it is encoded back (after UTF-8 validation) and passed
	 * into Poco::URI.
	 */
	static Poco::URI uri(const std::string &bytes,
			const unsigned long sizeLimit = SIZE_LIMIT_URI,
			const std::string &inputEncoding = "UTF-8");
};

}

#endif
