#pragma once

#include <string>

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class ServerEvent {
public:
	ServerEvent(
		const std::string &bind,
		const std::string &label);

	std::string bind() const;
	std::string label() const;

private:
	std::string m_bind;
	std::string m_label;
};

class ServerListener {
public:
	typedef Poco::SharedPtr<ServerListener> Ptr;

	virtual ~ServerListener();

	virtual void onUp(const ServerEvent &event);
	virtual void onDown(const ServerEvent &event);
};

}
