#pragma once

#include <functional>
#include <string>

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class RestFlow;

class RestAction {
public:
	typedef Poco::SharedPtr<RestAction> Ptr;
	typedef std::function<void(RestFlow &)> Call;

	RestAction(
		const std::string &handlerName,
		const std::string &actionName);
	virtual ~RestAction();

	void setCall(const Call &call);
	const Call &call() const;

	void setSessionRequired(bool session);
	bool sessionRequired() const;

	void setInputMaxSize(int bytes);
	int inputMaxSize() const;

	std::string handlerName() const
	{
		return m_handlerName;
	}

	std::string actionName() const
	{
		return m_actionName;
	}

	std::string fullName() const
	{
		return handlerName() + "." + actionName();
	}

	void execute(RestFlow &flow);

	static bool parseSessionRequired(const std::string &session);

private:
	std::string m_handlerName;
	std::string m_actionName;
	std::function<void(RestFlow &)> m_call;
	bool m_sessionRequired;
	int m_inputMaxSize;
};

}
