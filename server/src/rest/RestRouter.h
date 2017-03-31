#ifndef BEEEON_REST_ROUTER_H
#define BEEEON_REST_ROUTER_H

#include <map>
#include <vector>

#include <Poco/AutoPtr.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "rest/RestAction.h"
#include "rest/MappedRestAction.h"
#include "rest/RestLinker.h"
#include "rest/RestHandler.h"
#include "util/Loggable.h"

namespace BeeeOn {

class RestRouter : public RestLinker, Loggable {
public:
	RestRouter();
	~RestRouter();

	void setConfigurationFile(const std::string &file);
	void setApiName(const std::string &name);
	void addHandler(RestHandler *handler);

	void setApplicationName(const std::string &name);
	std::string applicationName() const override;

	MappedRestAction::Ptr route(
			const std::string &method,
			const Poco::URI &uri,
			MappedRestAction::Params &params) const;

	MappedRestAction::Ptr route(
			const std::string &method,
			const Poco::URI &uri) const;

	/**
	 * Lookup a rest action. The returned action is not necessarily
	 * mapped. It can also require params for execution. The purpose
	 * of this call is to enable access to internal routes that are
	 * not mapped to any URI.
	 */
	RestAction::Ptr lookup(
			const std::string &group,
			const std::string &action) const;

	/**
	 * Initialize routing after all handlers are registered
	 * and the configuration is set.
	 */
	void initRouting();

	/**
	 * Construct link to the API call identified as (group, action).
	 * If the call requires parameters, provide them as params.
	 */
	Poco::URI link(const std::string &group,
			const std::string &action,
			const std::vector<std::string> &params = {});

protected:
	void initHandler(RestHandler &handler,
			Poco::AutoPtr<Poco::Util::AbstractConfiguration> &config);
	void initAction(RestHandler &handler,
			const RestHandler::Action &action,
			Poco::AutoPtr<Poco::Util::AbstractConfiguration> &config);
	RestAction::Ptr createMapped(
		const std::string &handlerName,
		const RestHandler::Action &stub,
		const std::string &uri,
		Poco::AutoPtr<Poco::Util::AbstractConfiguration> &config,
		const std::string &base);

	std::string assureMethod(const std::string &method) const;

	std::string configMethod(
			Poco::AutoPtr<Poco::Util::AbstractConfiguration> &config,
			const std::string &base);
	bool configSessionRequired(
			Poco::AutoPtr<Poco::Util::AbstractConfiguration> &config,
			const std::string &base);
	std::string configCaching(
			Poco::AutoPtr<Poco::Util::AbstractConfiguration> &config,
			const std::string &base);
	int configInputMaxSize(
			Poco::AutoPtr<Poco::Util::AbstractConfiguration> &config,
			const std::string &base);

private:
	std::string m_name;
	std::string m_applicationName;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_config;
	std::vector<RestHandler *> m_handlers;
	std::map<std::string, RestAction::Ptr> m_linkMapper;
	std::map<std::string, 
		std::vector<MappedRestAction::Ptr>> m_methodMapper;
};

}

#endif
