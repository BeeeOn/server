#ifndef BEEEON_WORK_CONTENT_H
#define BEEEON_WORK_CONTENT_H

#include <string>

#include <Poco/JSON/Object.h>

#include "util/ClassInfo.h"

namespace BeeeOn {

class WorkContent {
public:
	WorkContent();
	WorkContent(const WorkContent &copy);
	WorkContent(const ClassInfo &type);
	WorkContent(const Poco::JSON::Object::Ptr content);

	void setType(const ClassInfo &type);
	ClassInfo type() const;

	std::string toString() const;

protected:
	Poco::JSON::Object::Ptr json() const;

private:
	ClassInfo m_type;
	Poco::JSON::Object::Ptr m_json;
};

}

#endif
