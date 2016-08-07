#ifndef BEEEON_NULL_DAO_H
#define BEEEON_NULL_DAO_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "Debug.h"

namespace BeeeOn {

template <
	/** Type (Entity) to work with. */
	typename T,
	/** Interface class. */
	typename P,
	/** Collection of T. */
	typename C = typename T::Collection,
	/** ID to identify T */
	typename ID = typename T::ID
	>
class NullDao : public P {
public:
	/**
	 * Get single object T identified by its unique ID.
	 */
	virtual typename T::Ptr get(ID id)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Test whether an object T with the given ID exists.
	 */
	virtual bool has(ID id)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Get a collection of objects T. It is possible to
	 * filter them by criteria.
	 */
	virtual C all(...)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Create object T and return a new ID for it.
	 */
	virtual ID create(const T &t)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Update the object T identified by the ID.
	 * Return the updated object.
	 */
	virtual typename T::Ptr update(ID id, const T &t)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Remove object T by ID. Return true if successfully
	 * removed. Otherwise, no such object exists. In case
	 * of error, an exception is thrown.
	 */
	virtual bool remove(ID id)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}
};

}

#endif
