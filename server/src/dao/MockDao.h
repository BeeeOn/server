#ifndef BEEEON_MOCK_DAO_H
#define BEEEON_MOCK_DAO_H

#include <map>
#include <vector>
#include <ostream>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "Debug.h"

namespace BeeeOn {

/**
 * Implements a memory-based Dao for a type T,
 * identificator ID.
 */
template <
	typename T,
	typename P,
	typename ID = typename T::ID
	>
class MockDao : public P {
public:
	typedef std::map<ID, typename T::Ptr> Storage;
	typedef typename Storage::iterator Iterator;

	MockDao()
	{
	}

	virtual bool fetch(T &t)
	{
		TRACE_METHOD();

		Iterator it = m_storage.find(t.id());

		if (it == m_storage.end())
			return false;

		t = T(t.id(), *it->second);
		return true;
	}

	virtual bool has(const T &t)
	{
		TRACE_METHOD();

		return m_storage.find(t.id()) != m_storage.end();
	}

	virtual bool insert(T &t)
	{
		TRACE_METHOD();

		if (t.id().isNull())
			return false;

		m_storage[t.id()] = new T(t.id(), t);

		return true;
	}

	virtual void create(T &t)
	{
		TRACE_METHOD();

		const ID id = nextID();
		m_storage[id] = new T(id, t);
		t = T(id, *m_storage[id]);
	}

	virtual bool update(T &t)
	{
		TRACE_METHOD();

		Iterator it = m_storage.find(t.id());

		if (it == m_storage.end())
			return false;

		it->second = new T(t.id(), t);
		t = T(t.id(), *m_storage[t.id()]);
		return true;
	}

	virtual bool remove(const T &t)
	{
		TRACE_METHOD();

		Iterator it = m_storage.find(t.id());

		if (it == m_storage.end())
			return false;

		m_storage.erase(it);
		return true;
	}

	Storage &storage()
	{
		return m_storage;
	}

protected:
	virtual ID nextID() = 0;

private:
	Storage m_storage;
};

}

#endif
