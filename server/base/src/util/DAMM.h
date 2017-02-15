#ifndef BEEEON_DAMM_H
#define BEEEON_DAMM_H

namespace BeeeOn {

/**
 * Error detection algorithm DAMM.
 * @see https://en.wikipedia.org/wiki/Damm_algorithm
 * @see https://en.wikibooks.org/wiki/Algorithm_Implementation/Checksums/Damm_Algorithm
 */
class DAMM {
public:
	/**
	 * @return check digit for the given string of digits
	 */
	static int compute(const std::string &s);

private:
	static const int table[10][10];
};

}

#endif
