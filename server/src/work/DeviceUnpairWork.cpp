#include "work/WorkAccess.h"
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
	setAttempt(attempt, WorkWriting(this, __FILE__, __LINE__));
}

void DeviceUnpairWork::setAttempt(unsigned int attempt, const WorkWriting &guard)
{
	guard.assureIs(this);
	m_attempt = attempt;
}

unsigned int DeviceUnpairWork::attempt() const
{
	return attempt(WorkReading(this, __FILE__, __LINE__));
}

unsigned int DeviceUnpairWork::attempt(const WorkAccess &guard) const
{
	guard.assureIs(this);
	return m_attempt;
}

void DeviceUnpairWork::setDevice(const Device &device)
{
	setDevice(device, WorkWriting(this, __FILE__, __LINE__));
}

void DeviceUnpairWork::setDevice(const Device &device, const WorkWriting &guard)
{
	guard.assureIs(this);
	m_device = device;
}

const Device &DeviceUnpairWork::device() const
{
	return device(WorkReading(this, __FILE__, __LINE__));
}

const Device &DeviceUnpairWork::device(const WorkAccess &guard) const
{
	guard.assureIs(this);
	return m_device;
}
