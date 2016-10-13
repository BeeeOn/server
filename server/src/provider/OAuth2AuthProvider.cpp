#include "provider/OAuth2AuthProvider.h"

using namespace std;
using namespace BeeeOn;

OAuth2AuthProvider::OAuth2AuthProvider(const string &name):
		AuthCodeAuthProvider(name)
{
	textInjector("client_id",
			(TextSetter) &OAuth2AuthProvider::setClientId);
	textInjector("client_secret",
			(TextSetter) &OAuth2AuthProvider::setClientSecret);
	textInjector("redirect_uri",
			(TextSetter) &OAuth2AuthProvider::setRedirectURI);
}
