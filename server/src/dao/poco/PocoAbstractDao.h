#ifndef BEEEON_POCO_ABSTRACT_DAO_H
#define BEEEON_POCO_ABSTRACT_DAO_H

#include <typeinfo>
#include <list>

#include <Poco/Logger.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>

#include "dao/EntityLoader.h"
#include "di/AbstractInjectorTarget.h"
#include "util/Loggable.h"

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoDaoManager;
class TransactionManager;
class SQLLoader;
class SQLQuery;

class PocoAbstractDao : public AbstractInjectorTarget,
		public EntityLoader,
		public Loggable {
public:
	PocoAbstractDao();
	virtual ~PocoAbstractDao();

	void setDaoManager(PocoDaoManager *manager);
	void setTransactionManager(TransactionManager *manager);
	void setSQLLoader(SQLLoader *loader);

protected:
	PocoDaoManager &manager();
	Poco::Data::Session session(bool transact = true);

	void registerQuery(SQLQuery &query);

	template <typename T>
	void assureHasId(const T &t)
	{
		if (t.id().isNull())
			throwMissingId(typeid(&t));
	}

	static double nanWhenEmpty(const Poco::Dynamic::Var &v);
	static std::string emptyWhenNull(const Poco::Dynamic::Var &v);
	static bool hasColumn(const Poco::Data::RecordSet &result, const std::string &name);
	static bool hasColumn(const Poco::Data::Row &result, const std::string &name);

	template <typename D>
	static D whenNull(const Poco::Dynamic::Var &v, const D def)
	{
		return v.isEmpty()? def : v.convert<D>();
	}

	std::size_t execute(Poco::Data::Statement &sql);
	void throwMissingId(const std::type_info &t);

	void loadQueries();

private:
	PocoDaoManager *m_manager;
	TransactionManager *m_transactionManager;
	SQLLoader *m_loader;
	std::list<SQLQuery *> m_queries;
};

}

#endif
