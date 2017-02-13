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

	template <typename P, typename T = InjectorTarget>
	void injector(const std::string key,
			void (P::*setter)(Poco::SharedPtr<T>))
	{
		m_refSetter.insert(std::make_pair(key,
				new SharedInjectorSetterImpl<P, T>(setter)));
	}

	void injectRef(const std::string &key, Poco::SharedPtr<InjectorTarget> value);
	void injectText(const std::string &key, const std::string &value);
	void injectNumber(const std::string &key, int value);

	virtual bool injectRefFallback(
			const std::string &key,
			Poco::SharedPtr<InjectorTarget> value);
	virtual bool injectTextFallback(
			const std::string &key,
			const std::string &value);
	virtual bool injectNumberFallback(
			const std::string &key,
			int value);

	virtual void injectionDone()
	{
	}

protected:
	RefSetterMap m_refSetter;
	TextSetterMap m_textSetter;
	NumberSetterMap m_numberSetter;
};

}

#endif
