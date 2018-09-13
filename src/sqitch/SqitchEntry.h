#pragma once

#include <string>

#include <Poco/Timestamp.h>

#include "sqitch/SqitchLine.h"

namespace BeeeOn {

class SqitchEntry : public SqitchLine {
public:
	typedef Poco::SharedPtr<SqitchEntry> Ptr;

	SqitchEntry(unsigned int line);

	void setName(const std::string &name);
	std::string name() const;

	void setTimestamp(const Poco::Timestamp &timestamp);
	Poco::Timestamp timestamp() const;

	void setAuthor(const std::string &author);
	std::string author() const;

	void setAuthorEmail(const std::string &email);
	std::string authorEmail() const;

private:
	std::string m_name;
	Poco::Timestamp m_timestamp;
	std::string m_author;
	std::string m_authorEmail;
};

}
