#ifndef BEEEON_DEPENDENCY_INJECTOR_H
#define BEEEON_DEPENDENCY_INJECTOR_H

#include <map>
#include <Poco/Exception.h>
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/AbstractConfiguration.h>
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

class InstanceInfo;

/**
 * DependencyInjector automates creation of application top-level
 * instances and glues them together. It is inspired by the Java
 * framework https://projects.spring.io/spring-framework/ but it
 * is highly simplified for C++ and for the current purposes.
 *
 * The instances are configured in a configuration file represented
 * by the Poco::AbstractConfiguration. The XMLConfiguration is
 * assumed. DependencyInjector handles tags:
 *
 * * instance: defines a new instance (singleton)
 *   + attribute name   - name of the instance
 *   + attribute class  - class name with namespace
 *
 * * set: sets a property of an instance
 *   + attribute name   - name of the property to inject
 *   + attribute ref    - value to set is a reference to an instance by name
 *   + attribute text   - value to set is a string
 *   + attribute number - value to set is an integer
 *
 * * add: adds a value to a property list
 *   + attribute name   - name of the property to inject
 *   + attribute ref    - value to add is a reference to an instance by name
 *   + attribute text   - value to add is a string
 *   + attribute number - value to add is an integer
 *
 * The attributes can contain ${property} strings which are expanded
 * automatically.
 *
 * No root element or parent entry is considered, so it is usually
 * necessary to pass a sub-configuration removing the initial prefix
 * by AbstractConfiguration::createView.
 *
 * Example configuration:
 *
 * <services>
 *  	<instance name="main" class="BeeeOn::Main">
 *		<set name="userService" ref="userService" />
 *		<set name="listenPort"  number="8080" />
 *		<add name="listeners"   ref="loginListener" />
 *		<add name="listeners"   ref="errorListener" />
 *	</instance>
 *
 *	<instance name="userService" class="BeeeOn::UserService" />
 *	<instance name="loginListener" class="BeeeOn::LoginListener" />
 *	<instance name="errorListener" class="BeeeOn::ErrorListener" />
 * </services>
 *
 * DependencyInjector di(config.createView("services"));
 * Main *main = di.create("main");
 */
class DependencyInjector {
public:
	typedef std::map<std::string, InjectorTarget *> InjectorSet;

	DependencyInjector(
		Poco::AutoPtr<Poco::Util::AbstractConfiguration> conf):
		m_conf(conf),
		m_logger(LOGGER_CLASS(this))
	{
	}

	~DependencyInjector();

	InjectorTarget *create(const std::string &name);

	template <typename T>
	T *create(const std::string &name)
	{
		return dynamic_cast<T *>(create(name));
	}

private:
	InjectorTarget *createNew(const InstanceInfo &info);
	InjectorTarget *injectDependencies(
			const InstanceInfo &info,
			InjectorTarget *target);
	void injectValue(const InstanceInfo &info,
			InjectorTarget *target,
			const std::string &key);
	bool tryInjectRef(const InstanceInfo &info,
			InjectorTarget *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectNumber(const InstanceInfo &info,
			InjectorTarget *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectText(const InstanceInfo &info,
			InjectorTarget *target,
			const std::string &key,
			const std::string &name);

private:
	InjectorSet m_set;
	Poco::Manifest<InjectorTarget> m_manifest;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_conf;
	Poco::Logger &m_logger;
};

}

#endif
