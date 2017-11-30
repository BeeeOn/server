#include "util/PocoStatementIntruder.h"

using namespace BeeeOn;

namespace Poco {
namespace Data {

class StatementIntruder : public Statement {
public:
	StatementImpl::Ptr accessImpl() const
	{
		return impl();
	}

	static const StatementIntruder &as(const Statement &stmt)
	{
		return static_cast<const StatementIntruder &>(stmt);
	}
};

class StatementImplIntruder : public StatementImpl {
public:
	AbstractBindingVec &accessBindings()
	{
		return bindings();
	}

	static StatementImplIntruder &as(StatementImpl &impl)
	{
		return static_cast<StatementImplIntruder &>(impl);
	}
};

}
}

PocoStatementImplIntruder::PocoStatementImplIntruder(
		Poco::Data::StatementImpl::Ptr impl):
	m_bindings(Poco::Data::StatementImplIntruder::as(*impl).accessBindings())
{
}

PocoStatementIntruder::PocoStatementIntruder(const Poco::Data::Statement &stmt):
	m_impl(Poco::Data::StatementIntruder::as(stmt).accessImpl())
{
}
