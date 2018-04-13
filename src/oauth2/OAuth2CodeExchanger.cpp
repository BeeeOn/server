#include "oauth2/OAuth2CodeExchanger.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

OAuth2CodeExchanger::~OAuth2CodeExchanger()
{
}

OAuth2CodeExchanger::Tokens OAuth2CodeExchanger::extractTokens(
	const Object::Ptr &body)
{
	const auto expires = body->getValue<unsigned int>("expires_in");

	return {
		body->optValue<string>("access_token", ""),
		body->optValue<string>("refresh_token", ""),
		body->optValue<string>("id_token", ""),
		body->getValue<string>("token_type"),
		Timespan(expires * Timespan::SECONDS)
	};
}
