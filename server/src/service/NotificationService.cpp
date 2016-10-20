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
	const User &user = identity.user();

	m_logger.information("first login of "
			+ identity.identity().email()
			+ ", "
			+ identity.provider()
			+ ", "
			+ identity.id().toString()
			+ ", "
			+ user.firstName()
			+ " "
			+ user.lastName(),
			__FILE__, __LINE__);

}
