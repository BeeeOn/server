#ifndef BEEEON_TEMPLATE_H
#define BEEEON_TEMPLATE_H

#include <istream>
#include <string>
#include <map>

namespace BeeeOn {

class Template {
public:
	Template(const std::string &text);
	Template(std::istream &in);

	const std::string &content() const
	{
		return m_content;
	}

	std::string apply(
		const std::map<std::string, std::string> &context);

protected:
	void subst(std::string &s, const std::string &key,
			const std::string &value);

private:
	std::string m_content;
};

}

#endif
