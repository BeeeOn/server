#ifndef BEEEON_INJECTOR_TARGET_H
#define BEEEON_INJECTOR_TARGET_H

#include <Poco/SharedPtr.h>
#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/Manifest.h>

namespace BeeeOn {

class DependencyInjector;

class InjectorTarget {
	friend DependencyInjector;
public:
	virtual ~InjectorTarget() {}

protected:
	virtual void injectRef(const std::string &key,
			Poco::SharedPtr<InjectorTarget> value) = 0;
	virtual void injectNumber(const std::string &key, int v) = 0;
	virtual void injectText(const std::string &key,
				const std::string &value) = 0;
	virtual void callHook(const std::string &hook, bool required = false) = 0;
};

class ManifestSingleton {
private:
	ManifestSingleton() {}
public:
	static ManifestSingleton *instance()
	{
		if (ManifestSingleton::singleton == NULL)
			ManifestSingleton::singleton = new ManifestSingleton();

		return ManifestSingleton::singleton;
	}

	static void destroy()
	{
		if (ManifestSingleton::singleton) {
			delete ManifestSingleton::singleton;
			ManifestSingleton::singleton = NULL;
		}
	}

	static Poco::Manifest<InjectorTarget> &manifest()
	{
		return ManifestSingleton::instance()->m_manifest;
	}

	static void reportInfo(Poco::Logger &logger);

private:
	Poco::Manifest<InjectorTarget> m_manifest;
	static ManifestSingleton *singleton;
};

/**
 * Register an object (inherited from InjectorTarget) to be available
 * for dependency injection.
 *
 * Arguments:
 * - name: a unique name among the system
 * - type: class name with namespace (e.g. BeeeOn::MagicService)
 */
#define BEEEON_OBJECT_IMPL(name, type)                  \
POCO_BEGIN_NAMED_MANIFEST(name, BeeeOn::InjectorTarget) \
POCO_EXPORT_CLASS(type)                                 \
POCO_END_MANIFEST                                       \
void name##_init() __attribute__((constructor));        \
void name##_init()                                      \
{                                                       \
	POCO_JOIN(pocoBuildManifest, name)(             \
		&BeeeOn::ManifestSingleton::manifest());\
}

#define _BEEEON_VA_EXPAND(x) x
#define _BEEEON_VA_COUNT_HELPER(_1, _2, _3, _4, _5, _6, _count, ...) _count
#define _BEEEON_VA_COUNT(...) \
	_BEEEON_VA_EXPAND(_BEEEON_VA_COUNT_HELPER(__VA_ARGS__, 6, 5, 4, 3, 2, 1))
#define _BEEEON_VA_SELECT_CAT(name, count, ...)\
	_BEEEON_VA_EXPAND(name##count(__VA_ARGS__))
#define _BEEEON_VA_SELECT_HELPER(name, count, ...) \
	_BEEEON_VA_SELECT_CAT(name, count, __VA_ARGS__)
#define _BEEEON_VA_SELECT(name, ...) \
	_BEEEON_VA_SELECT_HELPER(name, _BEEEON_VA_COUNT(__VA_ARGS__), __VA_ARGS__)

#define BEEEON_OBJECT1(cls) \
	BEEEON_OBJECT_IMPL(cls, cls)
#define BEEEON_OBJECT2(ns1, cls) \
	BEEEON_OBJECT_IMPL(ns1##_##cls, ns1::cls)
#define BEEEON_OBJECT3(ns1, ns2, cls) \
	BEEEON_OBJECT_IMPL(ns1##_##ns2##_##cls, ns1::ns2::cls)
#define BEEEON_OBJECT4(ns1, ns2, ns3, cls) \
	BEEEON_OBJECT_IMPL(ns1##_##ns2##_##ns3##_##cls, ns1::ns2::ns3::cls)

#define BEEEON_OBJECT(...) \
	_BEEEON_VA_SELECT(BEEEON_OBJECT, __VA_ARGS__)

}

#endif
