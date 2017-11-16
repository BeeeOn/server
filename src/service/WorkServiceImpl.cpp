#include <set>

#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/WorkServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, WorkServiceImpl)
BEEEON_OBJECT_CASTABLE(WorkService)
BEEEON_OBJECT_REF("facade", &WorkServiceImpl::setFacade)
BEEEON_OBJECT_END(BeeeOn, WorkServiceImpl)

using namespace Poco;
using namespace BeeeOn;

void WorkServiceImpl::setFacade(WorkFacade::Ptr facade)
{
	m_facade = facade;
}

bool WorkServiceImpl::fetch(Single<Work> &input)
{
	return m_facade->fetch(input.target(), input);
}

void WorkServiceImpl::list(Single<std::list<Work>> &input)
{
	std::set<Work> works;
	m_facade->fetch(works, input);

	input.target().assign(works.begin(), works.end());
}

void WorkServiceImpl::cancel(Single<Work> &input)
{
	m_facade->cancel(input.target(), input);
}

void WorkServiceImpl::remove(Single<Work> &input)
{
	if (!m_facade->remove(input.target(), input))
		throw NotFoundException("no such work " + input.target());
}
