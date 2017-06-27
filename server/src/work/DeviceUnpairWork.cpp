#include "work/DeviceUnpairWork.h"

using namespace Poco;
using namespace BeeeOn;

DeviceUnpairWork::DeviceUnpairWork():
	m_attempt(0)
{
}

DeviceUnpairWork::DeviceUnpairWork(const WorkID &id):
	Work(id),
	m_attempt(0)
{
}

void DeviceUnpairWork::setAttempt(unsigned int attempt)
{
	m_attempt = attempt;
}

unsigned int DeviceUnpairWork::attempt() const
{
	return m_attempt;
}

void DeviceUnpairWork::setDevice(const Device &device)
{
	m_device = device;
}

const Device &DeviceUnpairWork::device() const
{
	return m_device;
}
