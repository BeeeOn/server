#include <Poco/AbstractObserver.h>

#include "service/NotificationService.h"
#include "notification/FirstLoginNotification.h"
#include "model/VerifiedIdentity.h"
#include "Debug.h"

BEEEON_OBJECT(NotificationService, BeeeOn::NotificationService)

using namespace Poco;
using namespace BeeeOn;

NotificationService::NotificationService():
	m_logger(LOGGER_CLASS(this))
{
	injector<NotificationService, AbstractObserver>(
		"observer",
		&NotificationService::addObserver
	);
}

NotificationService::~NotificationService()
{
}

void NotificationService::addObserver(AbstractObserver *o)
{
	m_center.addObserver(*o);
}

void NotificationService::notifyFirstLogin(
		const VerifiedIdentity &identity)
{
	m_center.postNotification(new FirstLoginNotification(identity));
}
