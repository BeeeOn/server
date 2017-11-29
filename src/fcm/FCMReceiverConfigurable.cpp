#include "fcm/FCMReceiverConfigurable.h"

using namespace std;
using namespace BeeeOn;

FCMReceiverConfigurable::FCMReceiverConfigurable()
{
}

FCMReceiverConfigurable::~FCMReceiverConfigurable()
{
}

void FCMReceiverConfigurable::setSenderID(const string &senderID)
{
	m_senderID = senderID;
}

const string &FCMReceiverConfigurable::senderID() const
{
	return m_senderID;
}
