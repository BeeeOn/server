#include "service/NotificationService.h"
#include "Debug.h"

BEEEON_OBJECT(FakeNotificationService, BeeeOn::FakeNotificationService)

using namespace BeeeOn;

NotificationService::NotificationService():
	m_logger(LOGGER_CLASS(this))
{
}

void FakeNotificationService::notifyFirstLogin(
		const VerifiedIdentity &identity)
{
	m_logger.information("first login of "
			+ identity.identity().email()
			+ ", "
			+ identity.provider()
			+ ", "
			+ identity.id().toString(),
			__FILE__, __LINE__);

}
