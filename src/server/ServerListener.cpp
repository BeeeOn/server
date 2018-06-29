#include "server/ServerListener.h"

using namespace std;
using namespace BeeeOn;

ServerEvent::ServerEvent(
		const string &bind,
		const string &label):
	m_bind(bind),
	m_label(label)
{
}

string ServerEvent::bind() const
{
	return m_bind;
}

string ServerEvent::label() const
{
	return m_label;
}

ServerListener::~ServerListener()
{
}

void ServerListener::onUp(const ServerEvent &)
{
}

void ServerListener::onDown(const ServerEvent &)
{
}
