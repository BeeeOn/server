#include "dao/VerifiedIdentityDao.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoSQLVerifiedIdentityDao :
		public PocoAbstractDao,
		public VerifiedIdentityDao {
public:
	void create(VerifiedIdentity &identity) override;
	bool fetch(VerifiedIdentity &identity) override;
	bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider) override;
	void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email) override;
	bool update(VerifiedIdentity &identity) override;
	bool remove(const VerifiedIdentity &identity) override;

protected:
	void create(Poco::Data::Session &session, VerifiedIdentity &identity);
	bool fetch(Poco::Data::Session &session, VerifiedIdentity &identity);
	bool fetchBy(Poco::Data::Session &session, VerifiedIdentity &identity,
			const std::string &email,
			const std::string &provider);
	void fetchBy(Poco::Data::Session &session, 
			std::vector<VerifiedIdentity> &identities,
			const std::string &email);
	bool update(Poco::Data::Session &session, VerifiedIdentity &identity);
	bool remove(Poco::Data::Session &session, const VerifiedIdentity &identity);
};

}
