#ifndef BEEEON_SQITCH_PRAGMA_H
#define BEEEON_SQITCH_PRAGMA_H

#include <string>

#include "sqitch/SqitchLine.h"

namespace BeeeOn {

class SqitchPragma : public SqitchLine {
public:
	typedef Poco::SharedPtr<SqitchPragma> Ptr;

	SqitchPragma(unsigned int line);

	void setKey(const std::string &key);
	std::string key() const;

	void setValue(const std::string &value);
	std::string value() const;

	std::string toString() const override;

private:
	std::string m_key;
	std::string m_value;
};

}

#endif
