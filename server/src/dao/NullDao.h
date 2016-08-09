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
	 * Fetch a single object T.
	 */
	virtual bool fetch(T &t)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Test whether an object T exists in database.
	 */
	virtual bool has(const T &t)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Create object T in the database.
	 */
	virtual void create(T &t)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Update the object T in database.
	 * Return false if the object does not exists in database.
	 * In case of an error, an exception is thrown.
	 */
	virtual bool update(T &t)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}

	/**
	 * Remove object T from database. Return false if no such
	 * object exists. In case of an error, an exception is thrown.
	 */
	virtual bool remove(const T &t)
	{
		TRACE_METHOD();
		throw Poco::NotImplementedException(__func__);
	}
};

}

#endif
