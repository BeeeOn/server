#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include "dao/SociManager.h"

using namespace std;
using namespace soci;
using namespace Poco;
using namespace BeeeOn;

connection_pool &SociManager::pool()
{
	if (m_pool != NULL)
		return *m_pool;

	m_logger.information("initialize database (%s, %u)",
			m_backend, m_poolSize);

	m_pool = new connection_pool(m_poolSize);

	for (unsigned int i = 0; i < m_poolSize; ++i) {
		session &db = m_pool->at(i);

		m_logger.debug("connecting session %u", i);
		db.open(m_backend, m_conn);
	}

	m_logger.notice("database connected");

	return *m_pool;
}

void SociManager::injectionDone()
{
	if (m_script.empty()) {
		m_logger.debug("no initialization script, skipping");
		return;
	}

	m_logger.debug("executing initial script: " + m_script);

	FileInputStream script(m_script);
	string content;
	StreamCopier::copyToString(script, content);

	session db(pool());
	db.begin();
	db << (content);
	db.commit();

	m_logger.notice("database initialized successfully");
}

BEEEON_OBJECT(SociManager, BeeeOn::SociManager)
