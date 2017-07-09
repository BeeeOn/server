#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/User.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/ProfileRestHandler.h"
#include "restui/Serializing.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, ProfileRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_REF("identityService", &ProfileRestHandler::setIdentityService)
BEEEON_OBJECT_END(BeeeOn, RestUI, ProfileRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

ProfileRestHandler::ProfileRestHandler() :
		JSONRestHandler("profiles")
{
	registerAction<ProfileRestHandler>("current", &ProfileRestHandler::current);
	registerAction<ProfileRestHandler>("detail", &ProfileRestHandler::detail, {"profile_id"});
	registerAction<ProfileRestHandler>("list", &ProfileRestHandler::list);
}

void ProfileRestHandler::setIdentityService(IdentityService::Ptr service)
{
	m_identityService = service;
}

void ProfileRestHandler::detail(RestFlow &flow)
{
	VerifiedIdentity identity(VerifiedIdentityID::parse(flow.param("profile_id")));

	Single<VerifiedIdentity> input(identity);
	User user(flow.session()->userID());
	input.setUser(user);

	if (!m_identityService->fetch(input))
		throw NotFoundException("no such identity " + identity);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, identity);
	endSuccess(result);
}

void ProfileRestHandler::current(RestFlow &flow)
{
	VerifiedIdentity identity(flow.session()->identityID());

	flow.response().setStatus(303);
	flow.response()["Location"] = flow.linker().link("profiles", "detail", {identity.id().toString()});
}

void ProfileRestHandler::list(RestFlow &flow)
{
	User user(flow.session()->userID());
	std::list<VerifiedIdentity> profiles;

	Relation<std::list<VerifiedIdentity>, User> input(profiles, user);
	input.setUser(user);

	m_identityService->list(input);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, profiles);
	endSuccess(result);
}
