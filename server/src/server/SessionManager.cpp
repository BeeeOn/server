#include <Poco/Exception.h>

#include "server/SessionManager.h"
#include "util/Base64.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(SessionManager, BeeeOn::SessionManager)

const SessionID SessionManager::open(const User &user)
{
	if (user.email().empty())
		throw InvalidArgumentException("missing user e-mail");

	// lock here for the rest of the method
	RWLock::ScopedLock guard(m_lock, true);

	IDTable::const_iterator it;
	it = m_idTable.find(user.email());

	if (it != m_idTable.end())
		throw ExistsException("session already exists");

	char b[ID_LENGTH64];
	m_random->randomBytesUnlocked(b, sizeof(b));
	SessionID id = Base64::encode(b, sizeof(b));
	m_idTable.insert(make_pair(user.email(), id));
	m_table.insert(make_pair(id, user));

	return m_idTable.find(user.email())->second;
}

bool SessionManager::lookup(const SessionID &id, User &user)
{
	RWLock::ScopedLock guard(m_lock);

	Table::const_iterator it;
	it = m_table.find(id);

	if (it == m_table.end())
		return false;

	user = it->second;
	return true;
}

void SessionManager::close(const SessionID &id)
{
	RWLock::ScopedLock guard(m_lock, true);

	Table::const_iterator it;
	it = m_table.find(id);

	if (it == m_table.end())
		return;

	const User &user = it->second;

	IDTable::const_iterator email;
	email = m_idTable.find(user.email());
	if (email == m_idTable.end())
		return;

	m_table.erase(it);
	m_idTable.erase(email);
}
