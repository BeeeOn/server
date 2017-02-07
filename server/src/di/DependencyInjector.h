#ifndef BEEEON_DEPENDENCY_INJECTOR_H
#define BEEEON_DEPENDENCY_INJECTOR_H

#include <map>
#include <Poco/Exception.h>
#include <Poco/AutoPtr.h>
#include <Poco/SharedPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/AbstractConfiguration.h>
#include "di/InjectorTarget.h"
#include "util/Loggable.h"

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
 *   + attribute init   - lazy (default), early
 *
 * * set: sets a property of an instance
 *   + attribute name   - name of the property to inject
 *   + attribute ref    - value to set is a reference to an instance by name
 *   + attribute text   - value to set is a string
 *   + attribute number - value to set is an integer
 *   + attribute if-yes - optional condition to avoid applying the "set"
 *
 * * add: adds a value to a property list
 *   + attribute name   - name of the property to inject
 *   + attribute ref    - value to add is a reference to an instance by name
 *   + attribute text   - value to add is a string
 *   + attribute number - value to add is an integer
 *   + attribute if-yes - optional condition to avoid applying the "add"
 *
 * * alias: defines alias name of an existing instance
 *   + attribute name   - name of the alias (new instance name)
 *   + attribute ref    - name of an existing instance
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
 *	<instance name="stdoutErrorListener"
 * 			class="BeeeOn::StdoutErrorListener" />
 *	<alias name="errorListener" ref="stdoutErrorListener" />
 * </services>
 *
 * DependencyInjector di(config.createView("services"));
 * Main *main = di.create("main");
 */
class DependencyInjector : public Loggable {
public:
	typedef std::map<std::string, Poco::SharedPtr<InjectorTarget>> InjectorSet;
	typedef std::vector<Poco::SharedPtr<InjectorTarget>> InjectorVector;

	DependencyInjector(
		Poco::AutoPtr<Poco::Util::AbstractConfiguration> conf):
		m_conf(conf)
	{
		createEarly();
	}

	~DependencyInjector();

	Poco::SharedPtr<InjectorTarget> create(const std::string &name, bool disown = false);

	template <typename T>
	Poco::SharedPtr<T> create(const std::string &name, bool disown = false)
	{
		return create(name, disown).cast<T>();
	}

	Poco::SharedPtr<InjectorTarget> find(const std::string &name);

	template <typename T>
	Poco::SharedPtr<T> find(const std::string &name)
	{
		Poco::SharedPtr<InjectorTarget> t = find(name);
		return t.cast<T>();
	}

private:
	/**
	 * Create instances that should be always created.
	 */
	void createEarly();

	Poco::SharedPtr<InjectorTarget> createNoAlias(
			const InstanceInfo &info,
			bool disown = false);
	InjectorTarget *createNew(const InstanceInfo &info);
	Poco::SharedPtr<InjectorTarget> injectDependencies(
			const InstanceInfo &info,
			Poco::SharedPtr<InjectorTarget> target);
	void injectValue(const InstanceInfo &info,
			Poco::SharedPtr<InjectorTarget> target,
			const std::string &key,
			const std::string &name);
	bool tryInjectRef(const InstanceInfo &info,
			Poco::SharedPtr<InjectorTarget> target,
			const std::string &key,
			const std::string &name);
	bool tryInjectNumber(const InstanceInfo &info,
			Poco::SharedPtr<InjectorTarget> target,
			const std::string &key,
			const std::string &name);
	bool tryInjectText(const InstanceInfo &info,
			Poco::SharedPtr<InjectorTarget> target,
			const std::string &key,
			const std::string &name);

private:
	InjectorSet m_set;
	InjectorVector m_free;
	Poco::Manifest<InjectorTarget> m_manifest;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_conf;
};

}

#endif
