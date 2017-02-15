#ifndef BEEEON_DI_WRAPPER_H
#define BEEEON_DI_WRAPPER_H

#include <string>
#include <typeinfo>
#include <map>

#include <Poco/SharedPtr.h>
#include <Poco/ClassLibrary.h>
#include <Poco/Logger.h>

#include "util/Loggable.h"

namespace BeeeOn {

class DependencyInjector;
class DIWrapper;

/**
 * Duplicate injection helper has been detected.
 * This is a user bug, probably a duplicate setter entry.
 */
class DIWDuplicateException final : public std::exception {
public:
	DIWDuplicateException(const std::string &key,
			const std::type_info &method);

	const char *what() const noexcept override;

private:
	std::string m_text;
};

/**
 * Cannot cast in a setter. Probably, it can be solved
 * by a manual specification by BEEEON_OBJECT_CASTABLE.
 */
class DIWCastException final : public std::bad_cast {
public:
	DIWCastException(const std::type_info &from,
			const std::type_info &to);

	const char *what() const noexcept override;

private:
	std::string m_text;
};

/**
 * Common class to be inherited by all setters helpers.
 * The purpose is to be able to have all setters in a single map.
 */
struct DIWMethodHelper {
	virtual ~DIWMethodHelper();

	/**
	 * Get instance of the given wrapper.
	 */
	template <typename T, typename B>
	static B &extractInstance(DIWrapper &w);
};

/**
 * Interface to implementations of text setters.
 */
struct DIWTextSetter : public DIWMethodHelper {
	virtual ~DIWTextSetter();
	virtual void call(DIWrapper &b, const std::string &text) = 0;
};

template <typename T, typename B>
class DIWStringSetter final : public DIWTextSetter {
public:
	typedef void (B::*Setter)(const std::string &);

	DIWStringSetter(Setter setter):
		m_setter(setter)
	{
	}

	void call(DIWrapper &b, const std::string &text) override;
private:
	Setter m_setter;
};

/**
 * Interface to implementations of number setters.
 */
struct DIWNumberSetter : public DIWMethodHelper {
	virtual ~DIWNumberSetter();
	virtual void call(DIWrapper &b, long value) = 0;
};

template <typename T, typename B>
class DIWIntSetter final : public DIWNumberSetter {
public:
	typedef void (B::*Setter)(int value);

	DIWIntSetter(Setter setter):
		m_setter(setter)
	{
	}

	void call(DIWrapper &b, long value) override;
private:
	Setter m_setter;
};

template <typename T, typename B>
class DIWBoolSetter final : public DIWNumberSetter {
public:
	typedef void (B::*Setter)(bool value);

	DIWBoolSetter(Setter setter):
		m_setter(setter)
	{
	}

	void call(DIWrapper &b, long value) override;
private:
	Setter m_setter;
};

/**
 * Interface to implementations of object reference setter.
 */
struct DIWRefSetter : public DIWMethodHelper {
	virtual ~DIWRefSetter();
	virtual void call(DIWrapper &b, DIWrapper &i) = 0;

protected:
	template <typename I>
	Poco::SharedPtr<I> extractTarget(DIWrapper &i) const;
};

/**
 * Implement set via a raw pointer I*.
 */
template <typename T, typename B, typename I>
class DIWRawPtrSetter final : public DIWRefSetter {
public:
	typedef void (B::*Setter)(I *);

	DIWRawPtrSetter(Setter setter):
		m_setter(setter)
	{
	}

	void call(DIWrapper &b, DIWrapper &i) override;
private:
	Setter m_setter;
};

/**
 * Implement set via Poco::SharedPtr<I>.
 */
template <typename T, typename B, typename I>
class DIWSharedPtrSetter final : public DIWRefSetter {
public:
	typedef void (B::*Setter)(Poco::SharedPtr<I>);

	DIWSharedPtrSetter(Setter setter):
		m_setter(setter)
	{
	}

	void call(DIWrapper &b, DIWrapper &i) override;
private:
	Setter m_setter;
};

/**
 * Interface to a casting implementation. There is also a static
 * registry of possible casts so anybody can access it.
 * All casts register by DIWCast::add() are automatically deleted
 * on exit.
 */
struct DIWCast {
	virtual ~DIWCast();
	virtual bool wouldCast(const std::type_info &info) = 0;
	virtual bool isSame(DIWrapper &wrapper) = 0;
	virtual void cast(void *raw, void *dest) = 0;

	static void add(DIWCast *cast);
	static DIWCast *find(const std::type_info &info, DIWrapper &wrapper);
};

template <typename From, typename To>
struct DIWCastImpl final : public DIWCast {
	bool wouldCast(const std::type_info &info) override;
	bool isSame(DIWrapper &wrapper) override;
	void cast(void *raw, void *dest) override;
};

/**
 * Interface for hook calling implementations.
 */
struct DIWHook : public DIWMethodHelper {
	virtual ~DIWHook();
	virtual void call(DIWrapper &b) = 0;
};

/**
 * Call a hook with no arguments.
 */
template <typename T, typename B>
class DIWHookHandler final : public DIWHook {
public:
	typedef void (B::*Hook)();

	DIWHookHandler(Hook hook):
		m_hook(hook)
	{
	}

	void call(DIWrapper &b) override;

private:
	Hook m_hook;
};

/**
 * Interface to wrapper around a target instance. This wrapper can be
 * used to manage a collection of polymorphic wrappers around different
 * objects. All wrappers are expected to manage the underlying instance
 * via Poco::SharedPtr<?>.
 */
class DIWrapper {
	friend DependencyInjector;
public:
	virtual ~DIWrapper();

	/**
	 * Return pointer to Poco::SharedPtr<?>
	 */
	virtual void *raw() = 0;

	/**
	 * Return type of the underlying instance.
	 * This is typeid(?) from Poco::SharedPtr<?>.
	 */
	virtual const std::type_info &type() = 0;

protected:
	virtual void injectRef(const std::string &name,
			DIWrapper &wrapper) = 0;
	virtual void injectNumber(const std::string &name,
			int value) = 0;
	virtual void injectText(const std::string &name,
			const std::string &value) = 0;
	virtual void callHook(const std::string &name) = 0;
};

/**
 * Abstract implementation of wrapper. It holds the underlying
 * instance as Poco::SharedPtr<T>.
 */
template <typename T>
class AbstractDIWrapper : public DIWrapper, Loggable {
	friend DependencyInjector;
public:
	AbstractDIWrapper();
	virtual ~AbstractDIWrapper();

	/**
	 * Get shared pointer of the underlying instance.
	 */
	Poco::SharedPtr<T> instance();

	void *raw() override;
	const std::type_info &type() override;

protected:
	void injectRef(const std::string &name, DIWrapper &wrapper) override;
	void injectNumber(const std::string &name, int value) override;
	void injectText(const std::string &name, const std::string &value) override;
	void callHook(const std::string &name) override;

	template <typename B, typename I>
	void refSetter(const std::string &name, void (B::*setter)(I *));

	template <typename B, typename I>
	void refSetter(const std::string &name, void (B::*setter)(Poco::SharedPtr<I>));

	template <typename B>
	void numberSetter(const std::string &name, void (B::*setter)(int));

	template <typename B>
	void numberSetter(const std::string &name, void (B::*setter)(bool));

	template <typename B>
	void textSetter(const std::string &name, void (B::*setter)(const std::string &));

	template <typename B>
	void textSetter(const std::string &name, void (B::*setter)(const char *));

	template <typename B>
	void hookHandler(const std::string &name, void (B::*hook)());

	/**
	 * Generic install method implementation to be used by all other
	 * templated install methods. Installs the given DIWMethodHelper
	 * into the m_method map.
	 */
	void installMethod(const std::string &name, DIWMethodHelper *helper);

private:
	Poco::SharedPtr<T> m_instance;
	std::map<std::string, DIWMethodHelper *> m_method;
};

/**
 * Extract instance of the given wrapper. We expect that the underlying
 * wrapper holds an instance of T which is derived from B or T == B.
 * If the wrapper is not AbstractDIWrapper<T>, then std::bad_cast is
 * thrown.
 *
 * Usually, the method is called with *this as the DIWrapper argument
 * and thus we are sure it really is AbstractDIWrapper<T>.
 * Also, the B must be castable from T. This should be assured by
 * compile-time check during setter registration.
 */
template <typename T, typename B>
B &DIWMethodHelper::extractInstance(DIWrapper &w)
{
	AbstractDIWrapper<T> &wrapper = dynamic_cast<AbstractDIWrapper<T> &>(w);
	Poco::SharedPtr<T> instance = wrapper.instance();
	return dynamic_cast<B &>(*instance.get());
}

template <typename T, typename B>
void DIWStringSetter<T, B>::call(DIWrapper &b, const std::string &text)
{
	B &base = extractInstance<T, B>(b);
	(base.*m_setter)(text);
}

template <typename T, typename B>
void DIWIntSetter<T, B>::call(DIWrapper &b, long value)
{
	B &base = extractInstance<T, B>(b);
	(base.*m_setter)((int) value);
}

template <typename T, typename B>
void DIWBoolSetter<T, B>::call(DIWrapper &b, long value)
{
	B &base = extractInstance<T, B>(b);
	(base.*m_setter)(value != 0? true : false);
}

template <typename I>
Poco::SharedPtr<I> DIWRefSetter::extractTarget(DIWrapper &i) const
{
	DIWCast *cast = DIWCast::find(typeid(I), i);
	if (cast == NULL)
		throw DIWCastException(typeid(I), i.type());

	Poco::SharedPtr<I> inject;
	cast->cast(i.raw(), reinterpret_cast<void *>(&inject));

	return inject;
}

template <typename T, typename B, typename I>
void DIWRawPtrSetter<T, B, I>::call(DIWrapper &b, DIWrapper &i)
{
	B &base = extractInstance<T, B>(b);
	Poco::SharedPtr<I> inject = extractTarget<I>(i);
	(base.*m_setter)(inject.get());
}

template <typename T, typename B, typename I>
void DIWSharedPtrSetter<T, B, I>::call(DIWrapper &b, DIWrapper &i)
{
	B &base = extractInstance<T, B>(b);
	Poco::SharedPtr<I> inject = extractTarget<I>(i);
	(base.*m_setter)(inject);
}

/**
 * Test this cast can cast to the given type.
 */
template <typename From, typename To>
bool DIWCastImpl<From, To>::wouldCast(const std::type_info &info)
{
	return typeid(To) == info;
}

/**
 * Test this cast works with the same base type as the given wrapper.
 */
template <typename From, typename To>
bool DIWCastImpl<From, To>::isSame(DIWrapper &wrapper)
{
	return wrapper.type() == typeid(From);
}

/**
 * This is he most magical thing in the universe.
 * We get two pointers we *know* they point to Poco::SharedPtr<From>
 * and to Poco::SharedPtr<To> and do the cast by assigning them.
 *
 * This code can be executed only when wouldCast() and isSame() return true.
 */
template <typename From, typename To>
void DIWCastImpl<From, To>::cast(void *raw, void *dest)
{
	Poco::SharedPtr<From> *from = reinterpret_cast<Poco::SharedPtr<From> *>(raw);
	Poco::SharedPtr<To> *to = reinterpret_cast<Poco::SharedPtr<To> *>(dest);

	*to = *from;
}

template <typename T, typename B>
void DIWHookHandler<T, B>::call(DIWrapper &b)
{
	B &base = extractInstance<T, B>(b);
	(base.*m_hook)();
}

template <typename T>
AbstractDIWrapper<T>::AbstractDIWrapper():
	Loggable(typeid(T)),
	m_instance(new T)
{
}

template <typename T>
AbstractDIWrapper<T>::~AbstractDIWrapper()
{
	if (m_instance.referenceCount() > 1) {
		logger().warning(typeid(T).name()
			+ std::string(" is still referenced: ")
			+ std::to_string(m_instance.referenceCount()),
			__FILE__, __LINE__);
	}

	for (auto s : m_method)
		delete s.second;
}

template <typename T>
Poco::SharedPtr<T> AbstractDIWrapper<T>::instance()
{
	return m_instance;
}

template <typename T>
void *AbstractDIWrapper<T>::raw()
{
	return reinterpret_cast<void *>(&m_instance);
}

template <typename T>
const std::type_info &AbstractDIWrapper<T>::type()
{
	return typeid(T);
}

template <typename T>
void AbstractDIWrapper<T>::injectRef(
		const std::string &name,
		DIWrapper &wrapper)
{
	auto entry = m_method.find(name);
	if (entry == m_method.end()) {
		throw Poco::NotFoundException("missing ref property "
				+ name + " for type "
				+ typeid(T).name());
	}

	DIWRefSetter &setter = dynamic_cast<DIWRefSetter &>(*(entry->second));
	setter.call(*this, wrapper);
}

template <typename T>
void AbstractDIWrapper<T>::injectNumber(
		const std::string &name,
		int value)
{
	auto entry = m_method.find(name);
	if (entry == m_method.end()) {
		throw Poco::NotFoundException("missing number property "
				+ name + " for type "
				+ typeid(T).name());
	}


	DIWNumberSetter &setter = dynamic_cast<DIWNumberSetter &>(*(entry->second));
	setter.call(*this, value);
}

template <typename T>
void AbstractDIWrapper<T>::injectText(
		const std::string &name,
		const std::string &value)
{
	auto entry = m_method.find(name);
	if (entry == m_method.end()) {
		throw Poco::NotFoundException("missing text property "
				+ name + " for type "
				+ typeid(T).name());
	}

	DIWTextSetter &setter = dynamic_cast<DIWTextSetter &>(*(entry->second));
	setter.call(*this, value);
}

template <typename T>
void AbstractDIWrapper<T>::callHook(const std::string &name)
{
	auto entry = m_method.find(name);
	if (entry != m_method.end()) {
		if (logger().debug()) {
			logger().debug("no such hook '"
				+ name + "' defined for "
				+ typeid(T).name(),
				__FILE__, __LINE__);
		}

		DIWHook &handler = dynamic_cast<DIWHook &>(*(entry->second));
		handler.call(*this);
	}
}

template <typename T>
void AbstractDIWrapper<T>::installMethod(const std::string &name, DIWMethodHelper *helper)
{
	if (m_method.find(name) != m_method.end()) {
		delete helper;
		throw DIWDuplicateException(name, typeid(*helper));
	}

	m_method[name] = helper;
}

template <typename T> template <typename B, typename I>
void AbstractDIWrapper<T>::refSetter(
		const std::string &name,
		void (B::*setter)(I *))
{
	installMethod(name, new DIWRawPtrSetter<T, B, I>(setter));
}

template <typename T> template <typename B, typename I>
void AbstractDIWrapper<T>::refSetter(
		const std::string &name,
		void (B::*setter)(Poco::SharedPtr<I>))
{
	installMethod(name, new DIWSharedPtrSetter<T, B, I>(setter));
}

template <typename T> template <typename B>
void AbstractDIWrapper<T>::numberSetter(
		const std::string &name,
		void (B::*setter)(int))
{
	installMethod(name, new DIWIntSetter<T, B>(setter));
}

template <typename T> template <typename B>
void AbstractDIWrapper<T>::numberSetter(
		const std::string &name,
		void (B::*setter)(bool))
{
	installMethod(name, new DIWBoolSetter<T, B>(setter));
}

template <typename T> template <typename B>
void AbstractDIWrapper<T>::textSetter(
		const std::string &name,
		void (B::*setter)(const std::string &))
{
	installMethod(name, new DIWStringSetter<T, B>(setter));
}

template <typename T> template <typename B>
void AbstractDIWrapper<T>::hookHandler(
		const std::string &name,
		void (B::*hook)())
{
	installMethod(name, new DIWHookHandler<T, B>(hook));
}

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

	static Poco::Manifest<DIWrapper> &manifest()
	{
		return ManifestSingleton::instance()->m_manifest;
	}

	static void reportInfo(Poco::Logger &logger);

private:
	Poco::Manifest<DIWrapper> m_manifest;
	static ManifestSingleton *singleton;
};

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

#define BEEEON_OBJECT_IMPL(name, type)                  \
POCO_BEGIN_NAMED_MANIFEST(name, BeeeOn::DIWrapper)      \
POCO_EXPORT_CLASS(type)                                 \
POCO_END_MANIFEST                                       \
                                                        \
void name##_init() __attribute__((constructor));        \
void name##_init()                                      \
{                                                       \
	POCO_JOIN(pocoBuildManifest, name)(             \
		&BeeeOn::ManifestSingleton::manifest());\
}

#define BEEEON_WRAPPER(cls, wrapper) \
	struct wrapper final : public AbstractDIWrapper<cls> { \
		friend cls;                                    \
		using Self = cls;                              \
		wrapper()                                      \
		{                                              \
			DIWCast::add(new DIWCastImpl<cls, cls>);

#define BEEEON_OBJECT_BEGIN1(cls) \
BEEEON_WRAPPER(cls, cls##DIW)

#define BEEEON_OBJECT_BEGIN2(ns1, cls) \
namespace ns1 { \
BEEEON_WRAPPER(cls, cls##DIW)

#define BEEEON_OBJECT_BEGIN3(ns1, ns2, cls) \
namespace ns1 { namespace ns2 { \
BEEEON_WRAPPER(cls, cls##DIW)

#define BEEEON_OBJECT_BEGIN4(ns1, ns2, ns3, cls) \
namespace ns1 { namespace ns2 { namespace ns3 { \
BEEEON_WRAPPER(cls, cls##DIW)

#define BEEEON_OBJECT_BEGIN(...) \
	_BEEEON_VA_SELECT(BEEEON_OBJECT_BEGIN, __VA_ARGS__)
#define BEEEON_OBJECT_CASTABLE(to) \
	DIWCast::add(new DIWCastImpl<Self, to>);
#define BEEEON_OBJECT_REF(name, method) \
	refSetter(name, method);
#define BEEEON_OBJECT_NUMBER(name, method) \
	numberSetter(name, method);
#define BEEEON_OBJECT_TEXT(name, method) \
	textSetter(name, method);
#define BEEEON_OBJECT_HOOK(name, method) \
	hookHandler(name, method);

#define BEEEON_OBJECT_END1(cls) \
		}               \
	};                      \
BEEEON_OBJECT_IMPL(cls, cls##DIW)

#define BEEEON_OBJECT_END2(ns1, cls) \
		}                    \
	};                           \
}                                    \
BEEEON_OBJECT_IMPL(ns1##_##cls, ns1::cls##DIW)

#define BEEEON_OBJECT_END3(ns1, ns2, cls) \
		}                         \
	};                                \
}}                                        \
BEEEON_OBJECT_IMPL(ns1##_##ns2##_##cls, ns1::ns2::cls##DIW)

#define BEEEON_OBJECT_END4(ns1, ns2, ns3, cls) \
		}                              \
	};                                     \
}}}                                            \
BEEEON_OBJECT_IMPL(ns1##_##ns2##_##ns3##_##cls, ns1::ns2::ns3::cls##DIW)

#define BEEEON_OBJECT_END(...) \
	_BEEEON_VA_SELECT(BEEEON_OBJECT_END, __VA_ARGS__)

}

#endif
