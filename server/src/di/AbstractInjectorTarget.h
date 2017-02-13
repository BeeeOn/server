#ifndef BEEEON_ABSTRACT_INJECTOR_TARGET_H
#define BEEEON_ABSTRACT_INJECTOR_TARGET_H

#include <map>
#include <Poco/SharedPtr.h>

#include "di/InjectorTarget.h"

namespace BeeeOn {

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
	virtual void call(InjectorTarget *p, Poco::SharedPtr<InjectorTarget> t) = 0;
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
	void call(InjectorTarget *ip, Poco::SharedPtr<InjectorTarget> it)
	{
		P *p = dynamic_cast<P *>(ip);
		T *t = dynamic_cast<T *>(it.get());
		SetterFunc f = m_func;
		(p->*f)(t);
	}

private:
	SetterFunc m_func;
};

/**
 * SharedInjectorSetterImpl template class implements the InjectorSetter
 * with respect to the target types. It wraps calling a setter in
 * the form P::set(Poco::SharedPtr<T>).
 */
template <typename P, typename T>
class SharedInjectorSetterImpl : public InjectorSetter {
public:
	typedef void (P::*SetterFunc)(Poco::SharedPtr<T>);

	/**
	 * Create instance wrapping the given setter method.
	 */
	SharedInjectorSetterImpl(SetterFunc f):
		m_func(f)
	{
	}

	/**
	 * Call the ((P *) ip)->set(it.cast<T>()). Casted via dynamic_cast.
	 */
	void call(InjectorTarget *ip, Poco::SharedPtr<InjectorTarget> it)
	{
		P *p = dynamic_cast<P *>(ip);
		SetterFunc f = m_func;
		(p->*f)(it.cast<T>());
	}

private:
	SetterFunc m_func;
};

class NumberInjectorSetter {
public:
	virtual ~NumberInjectorSetter() {}

	virtual void call(InjectorTarget *ip, int value) = 0;
};

template <typename P>
class NumberInjectorSetterImpl : public NumberInjectorSetter {
public:
	typedef void (P::*SetterFunc)(int);

	NumberInjectorSetterImpl(SetterFunc func):
		m_func(func)
	{
	}

	void call(InjectorTarget *ip, int value) override
	{
		P *p = dynamic_cast<P *>(ip);
		SetterFunc f = m_func;
		(p->*f)(value);
	}

private:
	SetterFunc m_func;
};

class TextInjectorSetter {
public:
	virtual ~TextInjectorSetter() {}

	virtual void call(InjectorTarget *ip, const std::string &text) = 0;
};

template <typename P>
class TextInjectorSetterImpl : public TextInjectorSetter {
public:
	typedef void (P::*SetterFunc)(const std::string &);

	TextInjectorSetterImpl(SetterFunc func):
		m_func(func)
	{
	}

	void call(InjectorTarget *ip, const std::string &text) override
	{
		P *p = dynamic_cast<P *>(ip);
		SetterFunc f = m_func;
		(p->*f)(text);
	}

private:
	SetterFunc m_func;
};

class InjectorHook {
public:
	virtual ~InjectorHook() {}

	virtual void call(InjectorTarget *ip) = 0;
};

template <typename P>
class InjectorHookImpl : public InjectorHook {
public:
	typedef void (P::*Hook)();

	InjectorHookImpl(Hook hook):
		m_hook(hook)
	{
	}

	void call(InjectorTarget *ip) override
	{
		P *p = dynamic_cast<P *>(ip);
		Hook f = m_hook;
		(p->*f)();
	}

private:
	Hook m_hook;
};

class AbstractInjectorTarget : public InjectorTarget {
public:
	typedef std::map<std::string, InjectorSetter *> RefSetterMap;
	typedef std::map<std::string, TextInjectorSetter *> TextSetterMap;
	typedef std::map<std::string, NumberInjectorSetter *> NumberSetterMap;
	typedef std::map<std::string, InjectorHook *> HookMap;

	virtual ~AbstractInjectorTarget();

protected:
	template <typename T>
	void numberInjector(const std::string key, void (T::*setter)(int))
	{
		m_numberSetter.insert(std::make_pair(key,
				new NumberInjectorSetterImpl<T>(setter)));
	}

	template <typename T>
	void textInjector(const std::string key,
			void (T::*setter)(const std::string &))
	{
		m_textSetter.insert(std::make_pair(key,
				new TextInjectorSetterImpl<T>(setter)));
	}

	template <typename P, typename T = InjectorTarget>
	void injector(const std::string key,
			void (P::*setter)(T *))
	{
		m_refSetter.insert(std::make_pair(key,
				new InjectorSetterImpl<P, T>(setter)));
	}

	template <typename P, typename T = InjectorTarget>
	void injector(const std::string key,
			void (P::*setter)(Poco::SharedPtr<T>))
	{
		m_refSetter.insert(std::make_pair(key,
				new SharedInjectorSetterImpl<P, T>(setter)));
	}

	template <typename T>
	void hook(const std::string &key, void (T::*call)())
	{
		m_hookMap.insert(std::make_pair(key, new InjectorHookImpl<T>(call)));
	}

	void injectRef(const std::string &key, Poco::SharedPtr<InjectorTarget> value);
	void injectText(const std::string &key, const std::string &value);
	void injectNumber(const std::string &key, int value);
	void callHook(const std::string &key, bool required = false);

	virtual bool injectRefFallback(
			const std::string &key,
			Poco::SharedPtr<InjectorTarget> value);
	virtual bool injectTextFallback(
			const std::string &key,
			const std::string &value);
	virtual bool injectNumberFallback(
			const std::string &key,
			int value);

protected:
	RefSetterMap m_refSetter;
	TextSetterMap m_textSetter;
	NumberSetterMap m_numberSetter;
	HookMap m_hookMap;
};

}

#endif
