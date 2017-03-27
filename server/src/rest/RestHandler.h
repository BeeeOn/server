#ifndef BEEEON_REST_HANDLER_H
#define BEEEON_REST_HANDLER_H

#include <functional>
#include <string>
#include <vector>

#include "util/Loggable.h"

namespace BeeeOn {

class RestRouter;
class RestFlow;

class RestHandler : public Loggable {
	friend RestRouter;
public:
	typedef std::function<void(RestFlow &)> Handler;
	struct Action {
		std::string m_name;
		Handler m_handler;
		std::vector<std::string> m_params;
	};

	RestHandler(const std::string &name);
	virtual ~RestHandler();

protected:
	virtual Handler wrapHandler(const Handler &handler);

	template <typename T>
	void registerAction(const std::string &name,
			void (T::*method)(RestFlow &),
			const std::vector<std::string> &params = {})
	{
		const Handler handler = [this, method](RestFlow &flow) {
			T *t = static_cast<T *>(this);
			(t->*method)(flow);
		};

		registerAction(name, handler, params);
	}

	void registerAction(const std::string &name,
			Handler handler,
			const std::vector<std::string> &params = {});
	const std::vector<Action> &actions() const;
	const std::string &name() const;

private:
	std::string m_name;
	std::vector<Action> m_actions;
};

}

#endif
