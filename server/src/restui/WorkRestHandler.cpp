#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "rest/RestFlow.h"
#include "restui/Serializing.h"
#include "restui/WorkRestHandler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, WorkRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_REF("workService", &WorkRestHandler::setWorkService)
BEEEON_OBJECT_END(BeeeOn, RestUI, WorkRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

WorkRestHandler::WorkRestHandler():
	JSONRestHandler("work")
{
	registerAction<WorkRestHandler>("detail", &WorkRestHandler::detail, {"work_id"});
	registerAction<WorkRestHandler>("update", &WorkRestHandler::update, {"work_id"});
	registerAction<WorkRestHandler>("delete", &WorkRestHandler::remove, {"work_id"});
}

void WorkRestHandler::setWorkService(WorkService::Ptr service)
{
	m_service = service;
}

void WorkRestHandler::detail(RestFlow &flow)
{
	Work work(WorkID::parse(flow.param("work_id")));
	Single<Work> input(work);
	User user(flow.session()->userID());
	input.setUser(user);

	if (!m_service->fetch(input))
		throw NotFoundException("no such work " + work);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	endSuccess(result);
}

void WorkRestHandler::update(RestFlow &flow)
{
	Work work(WorkID::parse(flow.param("work_id")));
	Single<Work> input(work);
	User user(flow.session()->userID());
	input.setUser(user);

	const Object::Ptr object = parseInput(flow);

	const string &status = object->getValue<string>("status");
	if (status == "canceled")
		m_service->cancel(input);
	else
		throw InvalidArgumentException("unexpected status to update");

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, work);
	endSuccess(result);
}

void WorkRestHandler::remove(RestFlow &flow)
{
	Work work(WorkID::parse(flow.param("work_id")));
	Single<Work> input(work);
	User user(flow.session()->userID());
	input.setUser(user);

	m_service->remove(input);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 204);
	endSuccess(result);
}
