#ifndef BEEEON_POCO_SQL_GATEWAY_DAO_H
#define BEEEON_POCO_SQL_GATEWAY_DAO_H

#include "dao/DeviceDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "provider/DeviceInfoProvider.h"
#include "util/Loggable.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class DeviceInfo;

class PocoSQLDeviceDao :
		public PocoAbstractDao,
		public DeviceDao {
public:
	PocoSQLDeviceDao();

	void setDeviceInfoProvider(DeviceInfoProvider::Ptr provider);

	bool insert(Device &device, const Gateway &gateway) override;
	bool update(Device &device, const Gateway &gateway) override;
	bool fetch(Device &device, const Gateway &gateway) override;
	void fetchMany(std::list<Device> &devices) override;
	void fetchActiveBy(std::vector<Device> &devices,
			const Gateway &gateway) override;
	void fetchInactiveBy(std::vector<Device> &devices,
			const Gateway &gateway) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			Device &device, const Gateway &gateway,
			const DeviceInfoProvider &provider,
			const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			Device &device, const Gateway &gateway,
			const DeviceInfoProvider &provider,
			const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection,
			const Gateway &gateway, const DeviceInfoProvider &provider)
	{
		for (auto row : result) {
			Device device;

			if (!parseSingle(row, device, gateway, provider)) {
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(device);
		}
	}

protected:
	void assertTypeValid(const Device &device);

private:
	DeviceInfoProvider::Ptr m_deviceInfoProvider;

	SQLQuery m_queryInsert           {"devices.create"};
	SQLQuery m_queryUpdate           {"devices.update"};
	SQLQuery m_queryFetchFromGateway {"devices.fetch.from.gateway"};
	SQLQuery m_queryFetchActiveBy    {"devices.fetch.active.by.gateway"};
	SQLQuery m_queryFetchInactiveBy  {"devices.fetch.inactive.by.gateway"};
};

}

#endif
