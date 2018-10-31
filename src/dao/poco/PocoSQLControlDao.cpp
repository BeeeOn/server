#include <cmath>

#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLControlDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "model/Device.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLControlDao)
BEEEON_OBJECT_CASTABLE(ControlDao)
BEEEON_OBJECT_PROPERTY("daoManager", &PocoSQLControlDao::setDaoManager)
BEEEON_OBJECT_PROPERTY("transactionManager", &PocoSQLControlDao::setTransactionManager)
BEEEON_OBJECT_PROPERTY("sqlLoader", &PocoSQLControlDao::setQueryLoader)
BEEEON_OBJECT_PROPERTY("profiler", &PocoSQLControlDao::setQueryProfiler)
BEEEON_OBJECT_HOOK("done", &PocoSQLControlDao::loadQueries)
BEEEON_OBJECT_HOOK("cleanup", &PocoSQLControlDao::clearQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLControlDao)

PocoSQLControlDao::PocoSQLControlDao()
{
	registerQuery(m_queryFetchLast);
	registerQuery(m_queryInsertRequest);
	registerQuery(m_queryMarkAccepted);
	registerQuery(m_queryMarkFinished);
	registerQuery(m_queryCancelUnfinished);
}

void PocoSQLControlDao::assertTypeValid(const Device &device)
{
	if (device.type().isNull()) {
		throw InvalidArgumentException("missing device type for: "
				+ device.id().toString());
	}
}

bool PocoSQLControlDao::fetch(Control &control, const Device &device)
{
	// cannot assureHasId for control, SimpleID == 0 is allowed
	assureHasId(device);
	assertTypeValid(device);
	assureHasId(device.gateway());

	const auto deviceInfo = device.type();
	ModuleInfo moduleInfo;
	moduleInfo.setId(control.id());

	if (!deviceInfo->lookup(moduleInfo))
		return false;

	if (!moduleInfo.isControllable())
		return false;

	control.setInfo(moduleInfo);

	int id(control.id());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	Statement sql = (session() << m_queryFetchLast(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(id, "module_id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0) {
		Control::RequestedValue invalidRequested;
		ValueAt invalidRecent;

		control.setRequestedValue(invalidRequested);
		control.setRecentValue(invalidRecent);
		return true;
	}

	return parseSingle(result, control);
}

void PocoSQLControlDao::fetchBy(std::list<Control> &controls, const Device &device)
{
	assureHasId(device);
	assertTypeValid(device);
	assureHasId(device.gateway());

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	for (const auto &info : *device.type()) {
		if (!info.isControllable())
			continue;

		Control control(info.id());
		control.setInfo(info);

		int id(control.id());

		Statement sql = (session() << m_queryFetchLast(),
			use(gatewayID, "gateway_id"),
			use(deviceID, "device_id"),
			use(id, "module_id")
		);

		RecordSet result = executeSelect(sql);
		if (result.rowCount() == 0) {
			Control::RequestedValue invalidRequested;
			ValueAt invalidRecent;

			control.setRequestedValue(invalidRequested);
			control.setRecentValue(invalidRecent);

			controls.emplace_back(control);
		}
		else if (parseSingle(result, control)) {
			controls.emplace_back(control);
		}
		else {
			logger().warning("failed to fetch control " + control,
					__FILE__, __LINE__);
		}
	}
}

bool PocoSQLControlDao::insert(const Control::RequestedValue &request,
		const Control &control,
		const Device &device)
{
	// cannot assureHasId for control, SimpleID == 0 is allowed
	assureHasId(device);
	assertTypeValid(device);
	assureHasId(device.gateway());
	assureHasId(request.originator());

	if (!request.isValueValid())
		throw InvalidArgumentException("request must have a valid value");

	unsigned int moduleID(control.id());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());
	double value = request.value();

	if (request.requestedAt().isNull())
		throw IllegalStateException("requested timestamp must not be null");

	int64_t requestedAt = request.requestedAt().value().epochMicroseconds();

	Nullable<int64_t> acceptedAt;
	if (!request.acceptedAt().isNull())
		acceptedAt = request.acceptedAt().value().epochMicroseconds();

	Nullable<int64_t> finishedAt;
	if (!request.finishedAt().isNull())
		finishedAt = request.finishedAt().value().epochMicroseconds();

	bool failed = request.failed();
	string originatorId = request.originator().id().toString();

	Statement sql = (session() << m_queryInsertRequest(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(value, "value"),
		use(requestedAt, "requested_at"),
		use(acceptedAt, "accepted_at"),
		use(finishedAt, "finished_at"),
		use(failed, "failed"),
		use(originatorId, "originator_user_id")
	);

	return execute(sql) > 0;
}

bool PocoSQLControlDao::markAccepted(
		const Control::RequestedValue &request,
		const Control &control,
		const Device &device)
{
	// cannot assureHasId for control, SimpleID == 0 is allowed
	assureHasId(device);
	assertTypeValid(device);
	assureHasId(device.gateway());

	unsigned int moduleID(control.id());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	if (request.requestedAt().isNull())
		throw IllegalStateException("requested timestamp must not be null");

	int64_t requestedAt = request.requestedAt().value().epochMicroseconds();

	Nullable<int64_t> acceptedAt;
	if (!request.acceptedAt().isNull())
		acceptedAt = request.acceptedAt().value().epochMicroseconds();

	Statement sql = (session() << m_queryMarkAccepted(),
		use(acceptedAt, "accepted_at"),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(requestedAt, "requested_at")
	);

	return execute(sql) > 0;
}

bool PocoSQLControlDao::markFinished(
		const Control::RequestedValue &request,
		const Control &control,
		const Device &device)
{
	// cannot assureHasId for control, SimpleID == 0 is allowed
	assureHasId(device);
	assertTypeValid(device);
	assureHasId(device.gateway());

	unsigned int moduleID(control.id());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	if (request.requestedAt().isNull())
		throw IllegalStateException("requested timestamp must not be null");

	int64_t requestedAt = request.requestedAt().value().epochMicroseconds();

	Nullable<int64_t> finishedAt;
	if (!request.finishedAt().isNull())
		finishedAt = request.finishedAt().value().epochMicroseconds();

	bool failed = request.failed();

	Statement sql = (session() << m_queryMarkFinished(),
		use(finishedAt, "finished_at"),
		use(failed, "failed"),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(requestedAt, "requested_at")
	);

	return execute(sql) > 0;
}

size_t PocoSQLControlDao::cancelUnfinished()
{
	Statement sql = (session() << m_queryCancelUnfinished());
	return execute(sql);
}

bool PocoSQLControlDao::parseSingle(RecordSet &result,
		Control &control,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), control, prefix);
}

bool PocoSQLControlDao::parseSingle(Row &result,
		Control &control,
		const string &prefix)
{
	if (hasColumn(result, prefix + "module_id"))
		control.setId(ControlID::parse(result[prefix + "module_id"]));

	ValueAt recentValue;
	if (!result[prefix + "recent_at"].isEmpty()) {
		Nullable<Timestamp> at;
		if (!result[prefix + "recent_at"].isEmpty())
			at = Timestamp(result[prefix + "recent_at"].convert<int64_t>());

		recentValue.setAt(at);
		recentValue.setValue(result[prefix + "recent_value"]);
	}

	control.setRecentValue(recentValue);
	
	Control::RequestedValue requestedValue;
	if (!result[prefix + "requested_at"].isEmpty()) {
		requestedValue.setRequestedAt(
			Timestamp(result[prefix + "requested_at"].convert<int64_t>()));
	}

	if (!result[prefix + "requested_value"].isEmpty()) {
		requestedValue.setValue(
			result[prefix + "requested_value"].convert<double>());
	}

	if (!result[prefix + "accepted_at"].isEmpty()) {
		Nullable<Timestamp> at;
		if (!result[prefix + "accepted_at"].isEmpty())
			at = Timestamp(result[prefix + "accepted_at"].convert<int64_t>());

		requestedValue.setAcceptedAt(at);
	}

	if (!result[prefix + "finished_at"].isEmpty()) {
		Nullable<Timestamp> at;
		if (!result[prefix + "finished_at"].isEmpty())
			at = Timestamp(result[prefix + "finished_at"].convert<int64_t>());

		requestedValue.setFinishedAt(at);

		if (result[prefix + "failed"].convert<bool>())
			requestedValue.setResult(Control::RequestedValue::RESULT_FAILURE);
		else
			requestedValue.setResult(Control::RequestedValue::RESULT_SUCCESS);
	}
	else {
		requestedValue.setResult(Control::RequestedValue::RESULT_UNKNOWN);
	}

	if (!result[prefix + "originator_user_id"].isEmpty()) {
		User originator(UserID::parse(result[prefix + "originator_user_id"]));
		requestedValue.setOriginator(originator);
	}

	control.setRequestedValue(requestedValue);

	markLoaded(control);
	return true;
}
