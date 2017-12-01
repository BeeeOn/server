#pragma once

#include <Poco/Data/Statement.h>
#include <Poco/Data/StatementImpl.h>

namespace BeeeOn {

class PocoStatementImplIntruder {
public:
	PocoStatementImplIntruder(Poco::Data::StatementImpl::Ptr impl);

	Poco::Data::AbstractBindingVec &bindings()
	{
		return m_bindings;
	}

private:
	Poco::Data::AbstractBindingVec &m_bindings;
};

class PocoStatementIntruder {
public:
	PocoStatementIntruder(const Poco::Data::Statement &stmt);

	PocoStatementImplIntruder &impl()
	{
		return m_impl;
	}

private:
	PocoStatementImplIntruder m_impl;
};

}
