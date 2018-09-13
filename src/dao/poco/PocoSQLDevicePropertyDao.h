#pragma once

#include "dao/DevicePropertyDao.h"
#include "dao/Query.h"
#include "dao/poco/PocoAbstractDao.h"
#include "util/Loggable.h"

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
				Loggable::forMethod(__func__)
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
	Query m_queryInsert    {"devices_properties.insert"};
	Query m_queryUpdate    {"devices_properties.update"};
	Query m_queryRemove    {"devices_properties.remove"};
	Query m_queryFetch     {"devices_properties.fetch"};
	Query m_queryFetchByDevice {"devices_properties.fetch_by_device"};
};

}
