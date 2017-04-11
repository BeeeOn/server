#ifndef BEEEON_POCO_SQL_DEVICE_PROPERTY_DAO_H
#define BEEEON_POCO_SQL_DEVICE_PROPERTY_DAO_H

#include "dao/DevicePropertyDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "Debug.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLDevicePropertyDao :
		public PocoAbstractDao,
		public DevicePropertyDao {
public:
	PocoSQLDevicePropertyDao();

	bool insert(DeviceProperty &property, const Device &device) override;
	bool update(DeviceProperty &property, const Device &device) override;
	bool remove(const DeviceProperty &property, const Device &device) override;
	bool fetch(DeviceProperty &property, const Device &device) override;
	void fetchByDevice(std::list<DeviceProperty> &properties,
			const Device &device) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			DeviceProperty &property,
			const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			DeviceProperty &property,
			const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			DeviceProperty property;

			if (!parseSingle(row, property)) {
				LOGGER_FUNC(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(property);
		}
	}

protected:
	void assureKeyValid(const DeviceProperty &property) const;

private:
	SQLQuery m_queryInsert    {"devices_properties.insert"};
	SQLQuery m_queryUpdate    {"devices_properties.update"};
	SQLQuery m_queryRemove    {"devices_properties.remove"};
	SQLQuery m_queryFetch     {"devices_properties.fetch"};
	SQLQuery m_queryFetchByDevice {"devices_properties.fetch.by.device"};
};

}

#endif
