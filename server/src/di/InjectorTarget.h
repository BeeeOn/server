#ifndef BEEEON_INJECTOR_TARGET_H
#define BEEEON_INJECTOR_TARGET_H

#include <map>
#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>
#include <Poco/Manifest.h>

namespace BeeeOn {

class InjectorTarget {
public:
	virtual ~InjectorTarget() {}

protected:
	virtual void injectRef(const std::string &key,
			InjectorTarget *value) = 0;
	virtual void injectNumber(const std::string &key, int v) = 0;
	virtual void injectText(const std::string &key,
				const std::string &value) = 0;
};

/**
 * The class wraps calling of the setSomething methods. Each setter
 * method has its InjectorTarget class owner and a target type (must
 * implement InjectorTarget) that is to be set.
 * (e.g. CoolObject::setValue(CoolValue *v). The purpose of InjectorSetter
 * is to provide a common base class for creating vectors or maps
 * of heterogenous setters.
 */
class InjectorSetter {
public:
	InjectorSetter() {}
	virtual ~InjectorSetter() {}

	/**
	 * Perform p->set(t).
	 */
	virtual void call(InjectorTarget *p, InjectorTarget *t) = 0;
};

/**
 * InjectorSetterImpl template class implements the InjectorSetterImpl
 * with respect to the target types. It wraps calling a setter in
 * the form P::set(T *).
 */
template <typename P, typename T>
class InjectorSetterImpl : public InjectorSetter {
public:
	typedef void (P::*SetterFunc)(T *);

	/**
	 * Create instance wrapping the given setter method.
	 */
	InjectorSetterImpl(SetterFunc f):
		m_func(f)
	{
	}

	/**
	 * Call the ((P *) ip)->set((T *) it). Casted via dynamic_cast.
	 */
	void call(InjectorTarget *ip, InjectorTarget *it)
	{
		P *p = dynamic_cast<P *>(ip);
		T *t = dynamic_cast<T *>(it);
		SetterFunc f = m_func;
		(p->*f)(t);
	}

private:
	SetterFunc m_func;
};

class AbstractInjectorTarget : public InjectorTarget {
public:
	typedef void (AbstractInjectorTarget::*TextSetter)(
			const std::string &);
	typedef void (AbstractInjectorTarget::*NumberSetter)(int);

	typedef std::map<std::string, InjectorSetter *> RefSetterMap;
	typedef std::map<std::string, TextSetter> TextSetterMap;
	typedef std::map<std::string, NumberSetter> NumberSetterMap;

	virtual ~AbstractInjectorTarget();

protected:
	void numberInjector(const std::string key, NumberSetter setter)
	{
		m_numberSetter.insert(std::make_pair(key, setter));
	}

	void textInjector(const std::string key, TextSetter setter)
	{
		m_textSetter.insert(std::make_pair(key, setter));
	}

	template <typename P, typename T = InjectorTarget>
	void injector(const std::string key,
			void (P::*setter)(T *))
	{
		m_refSetter.insert(std::make_pair(key,
				new InjectorSetterImpl<P, T>(setter)));
	}

	void injectRef(const std::string &key, InjectorTarget *value);
	void injectText(const std::string &key, const std::string &value);
	void injectNumber(const std::string &key, int value);

protected:
	RefSetterMap m_refSetter;
	TextSetterMap m_textSetter;
	NumberSetterMap m_numberSetter;
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

#define BEEEON_OBJECT(name, type)                       \
POCO_BEGIN_NAMED_MANIFEST(name, BeeeOn::InjectorTarget) \
POCO_EXPORT_CLASS(type)                                 \
POCO_END_MANIFEST                                       \
void name##_init() __attribute__((constructor));        \
void name##_init()                                      \
{                                                       \
	POCO_JOIN(pocoBuildManifest, name)(             \
		&BeeeOn::ManifestSingleton::manifest());\
}

}

#endif
