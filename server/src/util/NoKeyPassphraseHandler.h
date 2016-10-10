#ifndef BEEEON_NO_KEY_PASSPHRASE_HANDLER_H
#define BEEEON_NO_KEY_PASSPHRASE_HANDLER_H

#include <Poco/Net/PrivateKeyPassphraseHandler.h>

namespace BeeeOn {

class NoKeyPassphraseHandler :
		public Poco::Net::PrivateKeyPassphraseHandler {
public:
	NoKeyPassphraseHandler(bool serverSide = true):
		Poco::Net::PrivateKeyPassphraseHandler(serverSide)
	{
	}

	void onPrivateKeyRequested(const void *sender, std::string &key)
	{
		key = "";
	}
};

}

#endif
