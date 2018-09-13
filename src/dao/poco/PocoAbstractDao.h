#pragma once

#include <typeinfo>
#include <list>

#include <Poco/Logger.h>
#include <Poco/Nullable.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>

#include "dao/EntityLoader.h"
#include "dao/QueryLoader.h"
#include "dao/QueryProfiler.h"
#include "transaction/TransactionManager.h"
#include "util/Loggable.h"

namespace Poco {

class Exception;

namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoDaoManager;
class Query;

class PocoAbstractDao :
		public EntityLoader,
		public Loggable {
public:
	PocoAbstractDao();
	virtual ~PocoAbstractDao();

	void setDaoManager(PocoDaoManager *manager);
	void setTransactionManager(TransactionManager::Ptr manager);
	void setQueryLoader(QueryLoader::Ptr loader);
	void setQueryProfiler(QueryProfiler::Ptr profiler);

	void loadQueries();
	void clearQueries();

protected:
	PocoDaoManager &manager();
	Poco::Data::Session session(bool transact = true);
	Poco::Data::Session openSession(bool transact = true);

	void registerQuery(Query &query);

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

	template <typename T>
	static Poco::Nullable<T> nullable(const Poco::Dynamic::Var &v)
	{
		return v.isEmpty()? Poco::Nullable<T>() : Poco::Nullable<T>(v.convert<T>());
	}

	std::size_t execute(Poco::Data::Statement &sql);
	Poco::Data::RecordSet executeSelect(Poco::Data::Statement &sql);
	void throwMissingId(const std::type_info &t);

private:
	PocoDaoManager *m_manager;
	TransactionManager::Ptr m_transactionManager;
	QueryLoader::Ptr m_loader;
	QueryProfiler::Ptr m_profiler;
	std::list<Query *> m_queries;
};

}
