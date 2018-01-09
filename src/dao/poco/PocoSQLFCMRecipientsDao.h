#pragma once

#include "dao/FCMRecipientsDao.h"
#include "dao/Query.h"

#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLFCMRecipientsDao :
		public PocoAbstractDao,
		public FCMRecipientsDao {
public:
	PocoSQLFCMRecipientsDao();

	void fetchBy(std::vector<FCMToken> &tokens, const Gateway &gateway) override;

private:
	Query m_queryByGateway {"fcm_recipients.by_gateway"};
};

}
