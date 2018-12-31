#pragma once

#include "dao/GatewayMessageDao.h"
#include "dao/Query.h"
#include "dao/poco/PocoAbstractDao.h"

namespace BeeeOn {

class PocoSQLGatewayMessageDao :
	public PocoAbstractDao,
	public GatewayMessageDao {
public:
	PocoSQLGatewayMessageDao();

	bool insert(GatewayMessage &message, size_t limit) override;
	bool remove(
		const GatewayMessage &message,
		const Gateway &gateway) override;
	bool fetch(
		GatewayMessage &message,
		const Gateway &gateway) override;
	void fetchBy(
		std::vector<GatewayMessage> &messages,
		const Gateway &gateway) override;

	static bool parseSingle(
		Poco::Data::RecordSet &result,
		GatewayMessage &message,
		const std::string &prefix = "");

	static bool parseSingle(
		Poco::Data::Row &result,
		GatewayMessage &message,
		const std::string &prefix = "");

	template <typename C>
	static void parseMany(
		Poco::Data::RecordSet &result,
		C &collection,
		const Gateway &gateway)
	{
		parseMany(result, collection);

		for (auto &message : collection)
			message.setGateway(gateway);
	}

	template <typename C>
	static void parseMany(
		Poco::Data::RecordSet &result,
		C &collection)
	{
		for (auto row : result) {
			GatewayMessage message;

			if (!parseSingle(row, message)) {
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.emplace_back(message);
		}
	}

private:
	Query m_queryInsert {"gateways_messages.insert"};
	Query m_queryRemove {"gateways_messages.remove"};
	Query m_queryFetch  {"gateways_messages.fetch_by_id"};
	Query m_queryFetchByGateway {"gateways_messages.fetch_by_gateway"};
	Query m_queryFetchByUser    {"gateways_messages.fetch_by_user"};
};

}
