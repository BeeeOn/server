#ifndef BEEEON_POCO_ABSTRACT_DAO_H
#define BEEEON_POCO_ABSTRACT_DAO_H

#include <typeinfo>

#include <Poco/Logger.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Statement.h>

#include "di/InjectorTarget.h"

namespace BeeeOn {

class PocoDaoManager;

class PocoAbstractDao : public AbstractInjectorTarget {
public:
	PocoAbstractDao();
	virtual ~PocoAbstractDao();

	void setDaoManager(PocoDaoManager *manager);

protected:
	PocoDaoManager &manager();

	template <typename T>
	void assureHasId(const T &t)
	{
		if (t.id().isNull())
			throwMissingId(typeid(&t));
	}

	double nanWhenEmpty(const Poco::Dynamic::Var &v) const;

	std::size_t execute(Poco::Data::Statement &sql);
	void throwMissingId(const std::type_info &t);

protected:
	Poco::Logger &m_logger;

private:
	PocoDaoManager *m_manager;
};

}

#endif
