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
 * Mock-specific collection of objects. It mimics EntityCollection.
 * It is intended for calls like Dao.all().
 */
template <
	typename T,
	typename C = typename T::Collection
	>
class MockCollection : public C {
public:
	typedef std::vector<typename T::Ptr> Vector;
	typedef typename Vector::const_iterator ConstIterator;

	void mockAdd(typename T::Ptr &t)
	{
		m_vec.push_back(t);
	}

	typename T::Ptr mockGet(unsigned int i)
	{
		return m_vec[i];
	}

	std::vector<typename T::Ptr> &mockDirect()
	{
		return m_vec;
	}

protected:
	void print(std::ostream &o) const
	{
		ConstIterator it;

		o << "[" << std::endl;
		for (it = m_vec.begin(); it != m_vec.end(); ++it) {
			o << *it << std::endl;
		}
		o << "]" << std::endl;
	}

private:
	Vector m_vec;
};

/**
 * Implements a memory-based Dao for a type T,
 * its collection C and identificator ID.
 */
template <
	typename T,
	typename P,
	typename C = typename T::Collection,
	typename ID = typename T::ID
	>
class MockDao : public P {
public:
	typedef std::map<ID, typename T::Ptr> Storage;
	typedef typename Storage::iterator Iterator;

	MockDao(): m_id(0)
	{
	}

	virtual typename T::Ptr get(ID id)
	{
		TRACE_METHOD();

		Iterator it = m_storage.find(id);

		if (it == m_storage.end())
			throw Poco::NotFoundException("no such ID ", id);

		return new T(id, *it->second);
	}

	virtual bool has(ID id)
	{
		TRACE_METHOD();

		return m_storage.find(id) != m_storage.end();
	}

	virtual C all(...)
	{
		TRACE_METHOD();

		MockCollection<T, C> all;
		Iterator it;

		for (it = m_storage.begin(); it != m_storage.end(); ++it)
			all.mockDirect().push_back(it->second);

		return all;
	}

	virtual ID create(const T &t)
	{
		TRACE_METHOD();

		const ID id = m_id++;
		m_storage.insert(std::make_pair(id, new T(id, t)));
		return id;
	}

	virtual typename T::Ptr update(ID id, const T &t)
	{
		TRACE_METHOD();

		Iterator it = m_storage.find(id);

		if (it == m_storage.end())
			throw Poco::NotFoundException("no such ID ", id);

		it->second = new T(id, t);
		return it->second;
	}

	virtual bool remove(ID id)
	{
		TRACE_METHOD();

		Iterator it = m_storage.find(id);

		if (it == m_storage.end())
			return false;

		m_storage.erase(it);
		return true;
	}

	Storage &storage()
	{
		return m_storage;
	}

private:
	Storage m_storage;
	ID m_id;
};

}

#endif
