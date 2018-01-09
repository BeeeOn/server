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
BEEEON_OBJECT_REF("daoManager", &PocoSQLControlDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLControlDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLControlDao::setIniFileQueryLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLControlDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLControlDao)

PocoSQLControlDao::PocoSQLControlDao()
{
	registerQuery(m_queryRecentState);
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
	assureHasId(control);
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

	Statement sql = (session() << m_queryRecentState(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(id, "control_id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0) {
		Control::State unknown;
		control.setCurrent(unknown);
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

		Statement sql = (session() << m_queryRecentState(),
			use(gatewayID, "gateway_id"),
			use(deviceID, "device_id"),
			use(id, "control_id")
		);

		RecordSet result = executeSelect(sql);
		if (result.rowCount() == 0) {
			Control::State unknown;
			control.setCurrent(unknown);

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

	Control::State tmp;
	if (!parseState(result, tmp, prefix + "current_state_"))
		throw IllegalStateException("at least current state is required");

	control.setCurrent(tmp);

	Nullable<Control::State> confirmed;
	if (parseState(result, tmp, prefix + "confirmed_state_"))
		confirmed = tmp;
	
	control.setLastConfirmed(confirmed);

	markLoaded(control);
	return true;
}


bool PocoSQLControlDao::parseState(Row &result,
		Control::State &state,
		const string &prefix)
{
	if (result[prefix + "at"].isEmpty())
		return false;

	state.setAt(Timestamp::fromEpochTime(result[prefix + "at"]));

	if (result[prefix + "value"].isEmpty())
		state.setValue(NAN);
	else
		state.setValue(result[prefix + "value"].convert<double>());

	if (result[prefix + "stability"].isEmpty())
		return false;

	const int stability = result[prefix + "stability"].convert<int>();
	if (stability >= 0 && stability < Control::State::_STABILITY_LAST)
		state.setStability((Control::State::Stability) stability);
	else
		throw IllegalStateException("invalid stability: " + to_string(stability));

	if (!result[prefix + "originator_user_id"].isEmpty()) {
		User user(UserID::parse(result[prefix + "originator_user_id"]));
		state.setOriginator(user);
	}

	return true;
}
