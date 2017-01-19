#ifndef BEEEON_MOCK_DAO_H
#define BEEEON_MOCK_DAO_H

#include <map>
#include <vector>
#include <ostream>
#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "dao/EntityLoader.h"
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
class MockDao : public P, public EntityLoader {
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
		markLoaded(t);
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

/**
 * Implements a memory-based Dao for a type T, its base type B,
 * identificator ID of T and BID of B.
 */
template <
	typename T,
	typename B,
	typename P,
	typename ID = typename T::ID,
	typename BID = typename B::ID
	>
class MockRelationDao : public P, public EntityLoader {
public:
	struct Key {
		ID id;
		BID bid;

		Key(const ID &a_id, const BID &a_bid):
			id(a_id), bid(a_bid)
		{
		}

		bool operator <(const Key &key) const
		{
			if (id == key.id)
				return bid < key.bid;
			else
				return id < key.id;
		}

		bool operator ==(const Key &key) const
		{
			return id == key.id && bid == key.bid;
		}

		bool operator >(const Key &key) const
		{
			if (id == key.id)
				return bid > key.bid;
			else
				return id > key.id;
		}
	};

	typedef std::map<Key, typename T::Ptr> Storage;
	typedef typename Storage::iterator Iterator;

	MockRelationDao()
	{
	}

	virtual bool fetch(T &t, const B &b)
	{
		TRACE_METHOD();

		Key key(t.id(), b.id());
		Iterator it = m_storage.find(key);

		if (it == m_storage.end())
			return false;

		t = T(t.id(), *it->second);
		setBase(t, b);
		markLoaded(t);
		return true;
	}

	virtual bool has(const T &t, const B &b)
	{
		TRACE_METHOD();

		Key key(t.id(), b.id());
		return m_storage.find(key) != m_storage.end();
	}

	virtual bool insert(T &t, const B &b)
	{
		TRACE_METHOD();

		if (t.id().isNull())
			return false;
		if (b.id().isNull())
			return false;

		Key key(t.id(), b.id());
		m_storage[key] = new T(t.id(), t);
		setBase(m_storage[key], b);

		return true;
	}

	virtual void create(T &t, const B &b)
	{
		TRACE_METHOD();

		const ID id = nextID();
		Key key(id, b.id());
		m_storage[key] = new T(id, t);
		setBase(m_storage[key], b);
		t = T(id, *m_storage[key]);
	}

	virtual bool update(T &t, const B &b)
	{
		TRACE_METHOD();

		Key key(t.id(), b.id());
		Iterator it = m_storage.find(key);

		if (it == m_storage.end())
			return false;

		it->second = new T(t.id(), t);
		setBase(it->second, b);
		t = T(t.id(), *m_storage[key]);
		return true;
	}

	virtual bool remove(const T &t, const B &b)
	{
		TRACE_METHOD();

		Key key(t.id(), b.id());
		Iterator it = m_storage.find(key);

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

	virtual void setBase(T &t, const B &b) = 0;

	virtual void setBase(typename T::Ptr ptr, const B &b)
	{
		setBase(*ptr, b);
	}


private:
	Storage m_storage;
};

}

#endif
