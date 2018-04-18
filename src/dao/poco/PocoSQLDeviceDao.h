#ifndef BEEEON_POCO_SQL_GATEWAY_DAO_H
#define BEEEON_POCO_SQL_GATEWAY_DAO_H

#include "dao/DeviceDao.h"
#include "dao/Query.h"
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
	void fetchActiveWithPrefix(std::vector<Device> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix) override;
	size_t removeUnused() override;

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

	Query m_queryInsert           {"devices.create"};
	Query m_queryUpdate           {"devices.update"};
	Query m_queryFetchFromGateway {"devices.fetch_from_gateway"};
	Query m_queryFetchActiveBy    {"devices.fetch_active_by_gateway"};
	Query m_queryFetchInactiveBy  {"devices.fetch_inactive_by_gateway"};
	Query m_queryFetchActiveWithPrefix {"devices.fetch_active_by_gateway_with_prefix"};
	Query m_queryRemoveUnused     {"devices.remove_unused"};
};

}

#endif
