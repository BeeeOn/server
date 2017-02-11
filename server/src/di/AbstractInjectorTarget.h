#ifndef BEEEON_ABSTRACT_INJECTOR_TARGET_H
#define BEEEON_ABSTRACT_INJECTOR_TARGET_H

#include <map>
#include <Poco/SharedPtr.h>

#include "di/InjectorTarget.h"

namespace BeeeOn {

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
