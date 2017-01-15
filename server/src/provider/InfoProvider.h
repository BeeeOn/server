#ifndef BEEEON_INFO_PROVIDER_H
#define BEEEON_INFO_PROVIDER_H

#include <set>

#include <Poco/SharedPtr.h>
#include <Poco/Path.h>
#include <Poco/Logger.h>
#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "util/SAXHelper.h"
#include "util/Loggable.h"

namespace BeeeOn {

template <typename T>
struct InfoProviderComparator {
	bool operator() (const Poco::SharedPtr<T> &a,
			const Poco::SharedPtr<T> &b) const
	{
		return *a < *b;
	}
};

template <typename T>
class InfoProvider : public Loggable {
public:
	typedef std::set<Poco::SharedPtr<T>,
			InfoProviderComparator<T>> InfoSet;
	virtual ~InfoProvider();

	const Poco::SharedPtr<T> findById(const typename T::ID &id) const;
	const Poco::SharedPtr<T> findByName(const std::string &name) const;

protected:
	bool registerInfo(const T &info);
	virtual void parseFile(const std::string &path,
			const std::string &infoLabel) = 0;

	virtual InfoSet &infoSet();
	virtual const InfoSet &infoSet() const;

private:
	InfoSet m_infoSet;
};

template <typename T>
InfoProvider<T>::~InfoProvider()
{
}

template <typename T>
typename InfoProvider<T>::InfoSet &InfoProvider<T>::infoSet()
{
	return m_infoSet;
}

template <typename T>
const typename InfoProvider<T>::InfoSet &InfoProvider<T>::infoSet() const
{
	return m_infoSet;
}

template <typename T>
const Poco::SharedPtr<T> InfoProvider<T>::findById(const typename T::ID &id) const
{
	Poco::SharedPtr<T> pattern(new T());
	pattern->setId(id);

	auto it = infoSet().find(pattern);
	if (it == infoSet().end())
		return NULL;

	return *it;
}

template <typename T>
const Poco::SharedPtr<T> InfoProvider<T>::findByName(const std::string &name) const
{
	for (auto info : infoSet()) {
		if (info.name() == name)
			return *info;
	}

	return NULL;
}

template <typename T>
bool InfoProvider<T>::registerInfo(const T &info)
{
	const Poco::SharedPtr<T> copy(new T(info));

	if (infoSet().find(copy) != infoSet().end())
		return false;

	infoSet().insert(copy);
	return true;
}

template <typename T>
class NullInfoProvider : public InfoProvider<T> {
public:
	static InfoProvider<T> &instance();

protected:
	void parseFile(const std::string &path,
			const std::string &infoLabel) override
	{
		throw Poco::NotImplementedException(__func__);
	}

	typename InfoProvider<T>::InfoSet &infoSet() override;
	const typename InfoProvider<T>::InfoSet &infoSet() const override;
};

template <typename T>
typename InfoProvider<T>::InfoSet &NullInfoProvider<T>::infoSet()
{
	throw Poco::NotImplementedException(__func__);
}

template <typename T>
const typename InfoProvider<T>::InfoSet &NullInfoProvider<T>::infoSet() const
{
	throw Poco::NotImplementedException(__func__);
}

template <typename T>
InfoProvider<T> &NullInfoProvider<T>::instance()
{
	static Poco::SingletonHolder<NullInfoProvider<T>> singleton;
	return *singleton.get();
}

}

#endif
