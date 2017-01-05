#ifndef BEEEON_POCO_SQL_GATEWAY_DAO_H
#define BEEEON_POCO_SQL_GATEWAY_DAO_H

#include "dao/DeviceDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "Debug.h"

namespace Poco {
namespace Data {

class Session;
class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLDeviceDao :
		public PocoAbstractDao,
		public DeviceDao {
public:
	PocoSQLDeviceDao();

	bool insert(Device &device, const Gateway &gateway) override;
	bool update(Device &device, const Gateway &gateway) override;
	bool fetch(Device &device, const Gateway &gateway) override;
	void fetchActiveBy(std::vector<Device> &devices,
			const Gateway &gateway) override;
	void fetchInactiveBy(std::vector<Device> &devices,
			const Gateway &gateway) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			Device &device, const Gateway &gateway,
			const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			Device &device, const Gateway &gateway,
			const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection,
			const Gateway &gateway)
	{
		for (auto row : result) {
			Device device;

			if (!parseSingle(row, device, gateway)) {
				LOGGER_FUNC(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(device);
		}
	}

protected:
	bool insert(Poco::Data::Session &session, Device &device,
			const Gateway &gateway);
	bool update(Poco::Data::Session &session, Device &device,
			const Gateway &gateway);
	bool fetch(Poco::Data::Session &session, Device &device,
			const Gateway &gateway);
	void fetchActiveBy(Poco::Data::Session &session,
			std::vector<Device> &devices,
			const Gateway &gateway);
	void fetchInactiveBy(Poco::Data::Session &session,
			std::vector<Device> &devices,
			const Gateway &gateway);

private:
	SQLQuery m_queryInsert           {"devices.create"};
	SQLQuery m_queryUpdate           {"devices.update"};
	SQLQuery m_queryFetchFromGateway {"devices.fetch.from.gateway"};
	SQLQuery m_queryFetchActiveBy    {"devices.fetch.active.by.gateway"};
	SQLQuery m_queryFetchInactiveBy  {"devices.fetch.inactive.by.gateway"};
};

}

#endif
