#include <Poco/Exception.h>

#include "server/SessionManager.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(SessionManager, BeeeOn::SessionManager)

const SessionID &SessionManager::open(const Info &info)
{
	Info::const_iterator email = info.find(INFO_EMAIL);
	if (email == info.end())
		throw InvalidArgumentException("missing INFO_EMAIL");

	// lock here for the rest of the method
	RWLock::ScopedLock guard(m_lock, true);

	IDTable::const_iterator it;
	it = m_idTable.find(email->second);

	if (it != m_idTable.end())
		throw ExistsException("session already exists");

	SessionID id = m_random->randomStringUnlocked(ID_LENGTH64);
	m_idTable.insert(make_pair(email->second, id));
	m_table.insert(make_pair(id, info));

	return m_idTable.find(email->second)->second;
}

bool SessionManager::update(const SessionID &id, const Info &info)
{
	RWLock::ScopedLock guard(m_lock, true);

	Table::iterator it;
	it = m_table.find(id);

	if (it == m_table.end())
		return false;

	if (info.find(INFO_EMAIL) == info.end())
		throw InvalidArgumentException("missing INFO_EMAIL");

	it->second = info;
	return true;
}

bool SessionManager::lookup(const SessionID &id, Info &info)
{
	RWLock::ScopedLock guard(m_lock);

	Table::const_iterator it;
	it = m_table.find(id);

	if (it == m_table.end())
		return false;

	info = it->second;
	return true;
}

void SessionManager::close(const SessionID &id)
{
	RWLock::ScopedLock guard(m_lock, true);

	Table::const_iterator it;
	it = m_table.find(id);

	if (it == m_table.end())
		return;

	const Info &info = it->second;

	IDTable::const_iterator email;
	email = m_idTable.find(info.find(INFO_EMAIL)->second);
	if (email == m_idTable.end())
		return;

	m_table.erase(it);
	m_idTable.erase(email);
}
