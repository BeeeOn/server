#include <list>
#include <map>

#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Event.h>
#include <Poco/Exception.h>
#include <Poco/Thread.h>
#include <Poco/Net/NetException.h>

#include "cppunit/BetterAssert.h"
#include "fcm/FCMClient.h"
#include "fcm/FCMMessage.h"
#include "fcm/FCMResponse.h"
#include "fcm/FCMSender.h"
#include "dao/FCMTokenDao.h"
#include "transaction/NoTransactionFactory.h"
#include "transaction/ThreadLocalTransactionManager.h"
#include "util/ExponentialBackOff.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class FCMSenderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FCMSenderTest);
	CPPUNIT_TEST(testJustSend);
	CPPUNIT_TEST(testFailingRecipient);
	CPPUNIT_TEST(testRenewedRecipient);
	CPPUNIT_TEST(testUndeliveredRecipient);
	CPPUNIT_TEST(testComplex);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();

	void testJustSend();
	void testFailingRecipient();
	void testRenewedRecipient();
	void testUndeliveredRecipient();
	void testComplex();

private:
	FCMClient::Ptr m_client;
	FCMTokenDao::Ptr m_dao;
	FCMSender m_sender;
	ThreadLocalTransactionManager m_transactionManager;
	SharedPtr<TransactionFactory> m_transactionFactory;
	BackOffFactory::Ptr m_backOff;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FCMSenderTest);

/**
 * FakeFCMClient stores sent messages and allows to specify responses
 * in advance. The final state can be analyzed.
 */
class FakeFCMClient : public FCMClient {
public:
	FCMResponse sendMessage(
			const FCMMessage &message,
			const string &serverKey,
			bool dryRun = false) override
	{
		m_eventWillSend.wait(5000);

		m_sentMessages.emplace_back(message);

		if (m_preparedResponses.empty()) {
			m_eventSent.set();
			throw IllegalStateException("no response");
		}

		const FCMResponse &response = m_preparedResponses.front();
		m_preparedResponses.pop_front();

		m_eventSent.set();
		return response;
	}

	void prepareResponse(const FCMResponse &response)
	{
		m_preparedResponses.emplace_back(response);
	}

	list<FCMResponse> &preparedResponses()
	{
		return m_preparedResponses;
	}

	list<FCMMessage> &sentMessages()
	{
		return m_sentMessages;
	}

	/**
	 * When a message is "sent" the event is set.
	 */
	Event &eventSent()
	{
		return m_eventSent;
	}

	/**
	 * Before every "send", the client is waiting here.
	 */
	Event &eventWillSend()
	{
		return m_eventWillSend;
	}

private:
	list<FCMResponse> m_preparedResponses;
	list<FCMMessage> m_sentMessages;
	Event m_eventSent;
	Event m_eventWillSend;
};

/**
 * FakeFCMTokenDao stores tokens into an internal std::map accessible
 * via the backend() method.
 */
class FakeFCMTokenDao : public FCMTokenDao {
public:
	void insert(FCMToken &token) override
	{
		m_backend.emplace(token.id(), token);
	}

	void fetchBy(vector<FCMToken> &tokens, const User &user) override
	{
		for (const auto &pair : m_backend) {
			if (pair.second.user().id() == user.id())
				tokens.emplace_back(pair.second);
		}
	}

	bool fetch(FCMToken &token) override
	{
		auto it = m_backend.find(token.id());
		if (it == m_backend.end())
			return false;

		token = it->second;
		return true;
	}

	bool replace(const FCMToken &from, FCMToken &to) override
	{
		m_backend.erase(from.id());
		m_backend.emplace(to.id(), to);
		return true;
	}

	bool remove(FCMToken &token) override
	{
		auto it = m_backend.find(token.id());
		if (it == m_backend.end())
			return false;

		m_backend.erase(it);
		return true;
	}

	map<FCMTokenID, FCMToken> &backend()
	{
		return m_backend;
	}

private:
	map<FCMTokenID, FCMToken> m_backend;
};

void FCMSenderTest::setUp()
{
	m_client = new FakeFCMClient;
	m_dao = new FakeFCMTokenDao;

	m_transactionFactory = new NoTransactionFactory;
	m_transactionManager.setFactory(m_transactionFactory);

	m_backOff = new ExponentialBackOffFactory;

	m_sender.setFCMClient(m_client);
	m_sender.setFCMTokenDao(m_dao);
	m_sender.setTransactionManager(&m_transactionManager);
	m_sender.setBackOffFactory(m_backOff);
}

/**
 * Very simple test. Just send a message and expect a successful response.
 */
void FCMSenderTest::testJustSend()
{
	FakeFCMClient &client = *m_client.cast<FakeFCMClient>();

	FCMMessage message;
	FCMResponse response;

	client.prepareResponse(response);
	CPPUNIT_ASSERT_EQUAL(1, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(0, client.sentMessages().size());

	client.eventWillSend().set();
	m_sender.send(message);

	CPPUNIT_ASSERT_EQUAL(0, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(1, client.sentMessages().size());
}

/**
 * Test sending a message with recipient that is not acceptable
 * by the messaging service. Such message is not resended. The
 * recipient must be deleted from the FCMTokenDao.
 */
void FCMSenderTest::testFailingRecipient()
{
	FakeFCMClient &client = *m_client.cast<FakeFCMClient>();
	FakeFCMTokenDao &dao = *m_dao.cast<FakeFCMTokenDao>();

	const FCMTokenID recipient = {"XXX"};
	FCMToken token(recipient);
	dao.backend().emplace(recipient, token);

	FCMMessage message;
	message.addRecipient(recipient);

	FCMResponse failing;
	failing.setFailingRecipients({recipient});
	client.prepareResponse(failing);

	CPPUNIT_ASSERT_EQUAL(1, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(0, client.sentMessages().size());
	CPPUNIT_ASSERT_EQUAL(1, dao.backend().size());

	client.eventWillSend().set();
	m_sender.send(message);
	client.eventSent().wait(5000);

	CPPUNIT_ASSERT_EQUAL(0, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(1, client.sentMessages().size());
	CPPUNIT_ASSERT_EQUAL(0, dao.backend().size());
}

/**
 * Test sending a message with recipient that has a more recent token.
 * The messaging service recommends us to replace it with the old one.
 * Finally, the old token must not be contained in the FCMTokenDao while
 * the new one is expected to be there. The message does not need to be
 * resended.
 */
void FCMSenderTest::testRenewedRecipient()
{
	FakeFCMClient &client = *m_client.cast<FakeFCMClient>();
	FakeFCMTokenDao &dao = *m_dao.cast<FakeFCMTokenDao>();

	const FCMTokenID recipient = {"XXX"};
	FCMToken token(recipient);
	dao.backend().emplace(recipient, token);

	FCMMessage message;
	message.addRecipient(recipient);

	const FCMTokenID renewedRecipient = {"YYY"};

	FCMResponse renewing;
	renewing.setRenewedRecipients({{recipient, renewedRecipient}});
	client.prepareResponse(renewing);

	CPPUNIT_ASSERT_EQUAL(1, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(0, client.sentMessages().size());
	CPPUNIT_ASSERT_EQUAL(1, dao.backend().size());
	CPPUNIT_ASSERT(dao.backend().find(recipient) != dao.backend().end());
	CPPUNIT_ASSERT(dao.backend().find(renewedRecipient) == dao.backend().end());

	client.eventWillSend().set();
	m_sender.send(message);
	client.eventSent().wait(5000);

	CPPUNIT_ASSERT_EQUAL(0, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(1, client.sentMessages().size());
	CPPUNIT_ASSERT_EQUAL(1, dao.backend().size());
	CPPUNIT_ASSERT(dao.backend().find(recipient) == dao.backend().end());
	CPPUNIT_ASSERT(dao.backend().find(renewedRecipient) != dao.backend().end());
}

/**
 * Send a message with recipient which is inaccessible but existing. Such message
 * could not be delivered and the messaging service refuses to cache it. Thus,
 * the FCMSender is expected to retry later. The second attempt succeeds.
 * Threading is necessary for this test.
 */
void FCMSenderTest::testUndeliveredRecipient()
{
	FakeFCMClient &client = *m_client.cast<FakeFCMClient>();
	FakeFCMTokenDao &dao = *m_dao.cast<FakeFCMTokenDao>();

	const FCMTokenID recipient = {"XXX"};
	FCMToken token(recipient);
	dao.backend().emplace(recipient, token);

	FCMMessage message;
	message.addRecipient(recipient);

	FCMResponse undelivered;
	undelivered.setUnderliveredRecipients({recipient});
	undelivered.setRetryAfter(20 * Timespan::MILLISECONDS);
	client.prepareResponse(undelivered);

	FCMResponse successful;
	client.prepareResponse(successful);

	CPPUNIT_ASSERT_EQUAL(2, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(0, client.sentMessages().size());
	CPPUNIT_ASSERT_EQUAL(1, dao.backend().size());

	Thread t;
	t.start(m_sender);

	client.eventWillSend().set();
	m_sender.send(message);
	client.eventSent().wait(5000);

	CPPUNIT_ASSERT_EQUAL(1, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(1, client.sentMessages().size());

	client.eventWillSend().set();
	client.eventSent().wait(5000);

	CPPUNIT_ASSERT_EQUAL(0, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(2, client.sentMessages().size());

	m_sender.stop();
	t.join(5000);

	CPPUNIT_ASSERT_EQUAL(1, dao.backend().size());
}

/**
 * Test a more complex scenario. There are 2 messages:
 *
 * A: with 3 recipients
 * B: with 1 recipient
 *
 * The recipients for A are: renewed, undeliverable, failing.
 * The recipient for B is undeliverable.
 *
 * We send the messages sequentially and waiting until the each
 * one is completely served (also with resends) to avoid a possible
 * non-determinism in the test.
 *
 * The A must be resended to 1 recipient. It causes to replace one
 * token and to delete one in the FCMTokenDao.
 *
 * The B must be resended once.
 */
void FCMSenderTest::testComplex()
{
	FakeFCMClient &client = *m_client.cast<FakeFCMClient>();
	FakeFCMTokenDao &dao = *m_dao.cast<FakeFCMTokenDao>();

	const FCMTokenID recipient0 = {"XXX"};
	FCMToken token0(recipient0);
	dao.backend().emplace(recipient0, token0);

	const FCMTokenID recipient1 = {"YYY"};
	FCMToken token1(recipient1);
	dao.backend().emplace(recipient1, token1);

	const FCMTokenID recipient2 = {"ZZZ"};
	FCMToken token2(recipient2);
	dao.backend().emplace(recipient2, token2);

	const FCMTokenID recipient3 = {"000"};
	FCMToken token3(recipient3);
	dao.backend().emplace(recipient3, token3);

	FCMMessage messageA;
	messageA.addRecipient(recipient0);
	messageA.addRecipient(recipient1);
	messageA.addRecipient(recipient2);

	FCMMessage messageB;
	messageB.addRecipient(recipient3);

	FCMResponse undeliveredA;
	undeliveredA.setUnderliveredRecipients({recipient1});
	undeliveredA.setFailingRecipients({recipient2});
	undeliveredA.setRenewedRecipients({{recipient0, FCMTokenID{"someother"}}});
	undeliveredA.setRetryAfter(20 * Timespan::MILLISECONDS);
	client.prepareResponse(undeliveredA);

	FCMResponse undeliveredB;
	undeliveredB.setUnderliveredRecipients({recipient3});
	undeliveredB.setRetryAfter(20 * Timespan::MILLISECONDS);
	client.prepareResponse(undeliveredB);

	FCMResponse successA;
	client.prepareResponse(successA);

	FCMResponse successB;
	client.prepareResponse(successB);

	CPPUNIT_ASSERT_EQUAL(4, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(0, client.sentMessages().size());
	CPPUNIT_ASSERT_EQUAL(4, dao.backend().size());

	Thread t;
	t.start(m_sender);

	client.eventWillSend().set();
	m_sender.send(messageA);
	client.eventSent().wait(1000);

	client.eventWillSend().set();
	client.eventSent().wait(5000);

	CPPUNIT_ASSERT_EQUAL(2, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(2, client.sentMessages().size());

	CPPUNIT_ASSERT_EQUAL(3, dao.backend().size());
	CPPUNIT_ASSERT(dao.backend().find(recipient0) == dao.backend().end());
	CPPUNIT_ASSERT(dao.backend().find(recipient1) != dao.backend().end());
	CPPUNIT_ASSERT(dao.backend().find(recipient2) == dao.backend().end());
	CPPUNIT_ASSERT(dao.backend().find(recipient3) != dao.backend().end());
	CPPUNIT_ASSERT(dao.backend().find(FCMTokenID{"someother"}) != dao.backend().end());

	client.eventWillSend().set();
	m_sender.send(messageB);
	client.eventSent().wait(5000);

	client.eventWillSend().set();
	client.eventSent().wait(5000);

	CPPUNIT_ASSERT_EQUAL(0, client.preparedResponses().size());
	CPPUNIT_ASSERT_EQUAL(4, client.sentMessages().size());

	m_sender.stop();
	t.join(5000);

	CPPUNIT_ASSERT_EQUAL(3, dao.backend().size());
}

}
