#ifndef BEEEON_POCO_SQL_CONTROL_DAO_H
#define BEEEON_POCO_SQL_CONTROL_DAO_H

#include "dao/ControlDao.h"
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

class DeviceInfo;

class PocoSQLControlDao :
		public PocoAbstractDao,
		public ControlDao {
public:
	PocoSQLControlDao();

	bool fetch(Control &control,
			const Device &device) override;
	void fetchBy(std::list<Control> &controls,
			const Device &device) override;
	bool insert(const Control::RequestedValue &request,
			const Control &control,
			const Device &device) override;
	bool update(const Control::RequestedValue &request,
			const Control &control,
			const Device &device) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			Control &control,
			const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			Control &control,
			const std::string &prefix = "");

protected:
	void assertTypeValid(const Device &device);

private:
	Query m_queryFetchLast     {"controls_fsm.fetch_last"};
	Query m_queryInsertRequest {"controls_fsm.insert_request"};
	Query m_queryUpdateRequest {"controls_fsm.update_request"};
};

}

#endif

