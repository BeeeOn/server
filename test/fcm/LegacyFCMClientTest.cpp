#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Environment.h>
#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>

#include "cppunit/BetterAssert.h"
#include "fcm/LegacyFCMClient.h"
#include "fcm/FCMMessage.h"
#include "fcm/FCMResponse.h"
#include "ssl/SSLClient.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class LegacyFCMClientTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(LegacyFCMClientTest);
	CPPUNIT_TEST(testParseRetryAfter);
	CPPUNIT_TEST(testMissingRecipients);
	CPPUNIT_TEST_SUITE_ADD_CUSTOM_TESTS(addOnlineTests);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();

	void testParseRetryAfter();
	void testMissingRecipients();
	void testInvalidRecipient();
	void testSuccessful();

	static void addOnlineTests(CppUnit::TestSuiteBuilderContext<LegacyFCMClientTest> &context)
	{
		if (Environment::has("FCM_SERVER_KEY")) {
			context.addTest(new CppUnit::TestCaller<LegacyFCMClientTest>(
				context.getTestNameFor("testInvalidRecipient"),
				&LegacyFCMClientTest::testInvalidRecipient,
				context.makeFixture()
			));

			if (Environment::has("FCM_RECIPIENT")) {
				context.addTest(new CppUnit::TestCaller<LegacyFCMClientTest>(
					context.getTestNameFor("testSuccessful"),
					&LegacyFCMClientTest::testSuccessful,
					context.makeFixture()
				));
			}
		}
	}

private:
	LegacyFCMClient m_client;
	SSLClient::Ptr m_sslClient;
	string m_serverKey;
	FCMTokenID m_recipient;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LegacyFCMClientTest);

class TestableLegacyFCMClient : public LegacyFCMClient {
public:
	using LegacyFCMClient::parseRetryAfter;
};

void LegacyFCMClientTest::setUp()
{
	m_serverKey = Environment::get("FCM_SERVER_KEY", "");
	m_recipient = FCMTokenID::parse(Environment::get("FCM_RECIPIENT", ""));

	m_sslClient = new SSLClient;
	m_sslClient->setVerificationMode("none");
	m_client.setSSLConfig(m_sslClient);
}

/**
 * Test we parse Retry-After header properly. It can be specified either
 * as a number of seconds or as full date time according to the HTTP
 * Date format. On invalid input, we expect not to fail but receiving
 * an invalid time delay.
 */
void LegacyFCMClientTest::testParseRetryAfter()
{
	TestableLegacyFCMClient client;
	Timespan tmp;

	tmp = client.parseRetryAfter("120");
	CPPUNIT_ASSERT_EQUAL(120, tmp.totalSeconds());

	tmp = client.parseRetryAfter("241");
	CPPUNIT_ASSERT_EQUAL(241, tmp.totalSeconds());

	tmp = client.parseRetryAfter("  1");
	CPPUNIT_ASSERT_EQUAL(1, tmp.totalSeconds());

	tmp = client.parseRetryAfter("-1");
	CPPUNIT_ASSERT(tmp < 0);

	tmp = client.parseRetryAfter("0");
	CPPUNIT_ASSERT_EQUAL(0, tmp.totalSeconds());

	tmp = client.parseRetryAfter("Wed, 21 Oct 2015 07:28:00 GMT");
	CPPUNIT_ASSERT(tmp == FCMResponse::UNKNOWN_DELAY);

	// Fri, 24 Nov 2017 10:51:50 +0100
	const Timestamp at = Timestamp::fromEpochTime(1511517110);

	tmp = client.parseRetryAfter("Fri, 24 Nov 2017 10:51:50 +0100", at);
	CPPUNIT_ASSERT_EQUAL(0, tmp.totalSeconds());

	tmp = client.parseRetryAfter("Fri, 24 Nov 2017 10:59:51 +0100", at);
	CPPUNIT_ASSERT_EQUAL(8 * 60 + 1, tmp.totalSeconds());

	tmp = client.parseRetryAfter("illegal input");
	CPPUNIT_ASSERT(tmp < 0);

	tmp = client.parseRetryAfter("209F");
	CPPUNIT_ASSERT(tmp < 0);

	tmp = client.parseRetryAfter("a209F");
	CPPUNIT_ASSERT(tmp < 0);

	tmp = client.parseRetryAfter("broken Fri, 24 Nov 2017 10:59:51 +0100", at);
	CPPUNIT_ASSERT(tmp < 0);
}

/**
 * Test that LegacyFCMClient fails to send a notification with no recipient.
 */
void LegacyFCMClientTest::testMissingRecipients()
{
	FCMMessage message;

	message.setTitle("Missing a recipient");
	message.setBody("Testing body");
	message.setTimeToLive(0);
	message.setContentAvailable(true);
	message.setPriority(FCMMessage::PRIORITY_NORM);

	CPPUNIT_ASSERT_THROW(
		m_client.sendMessage(message, m_serverKey),
		IllegalStateException
	);
}

/**
 * Test processing of response while sending a notification to an invalid
 * recipient.
 */
void LegacyFCMClientTest::testInvalidRecipient()
{
	FCMMessage message;

	message.setTitle("Invalid recipient");
	message.setBody("Testing body");
	message.setTimeToLive(0);
	message.setContentAvailable(true);
	message.setPriority(FCMMessage::PRIORITY_NORM);
	message.addRecipient(FCMTokenID::parse("xeorjgoierjgergergeght"));

	const FCMResponse &response = m_client.sendMessage(message, m_serverKey);

	CPPUNIT_ASSERT_EQUAL(0, response.undeliveredRecipients().size());
	CPPUNIT_ASSERT_EQUAL(1, response.failingRecipients().size());
	CPPUNIT_ASSERT_EQUAL("xeorjgoierjgergergeght", response.failingRecipients().begin()->toString());
	CPPUNIT_ASSERT_EQUAL(0, response.renewedRecipients().size());
}

/**
 * Test successful FCM notification pass.
 */
void LegacyFCMClientTest::testSuccessful()
{
	FCMMessage message;

	message.setTitle("Successful");
	message.setBody("Testing body");
	message.setTimeToLive(0);
	message.setContentAvailable(true);
	message.setPriority(FCMMessage::PRIORITY_NORM);
	message.addRecipient(m_recipient);

	const FCMResponse &response = m_client.sendMessage(message, m_serverKey);

	CPPUNIT_ASSERT_EQUAL(0, response.undeliveredRecipients().size());
	CPPUNIT_ASSERT_EQUAL(0, response.failingRecipients().size());
	CPPUNIT_ASSERT_EQUAL(0, response.renewedRecipients().size());
}

}
