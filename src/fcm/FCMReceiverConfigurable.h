#pragma once

#include <string>

namespace BeeeOn {

class FCMReceiverConfigurable {
public:
	FCMReceiverConfigurable();
	virtual ~FCMReceiverConfigurable();

	void setSenderID(const std::string &senderID);
	const std::string &senderID() const;

private:
	std::string m_senderID;
};

}
