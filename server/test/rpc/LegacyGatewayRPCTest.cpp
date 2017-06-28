#include <cppunit/extensions/HelperMacros.h>

#include <Poco/SAX/SAXException.h>
#include <Poco/DOM/Document.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/SharedPtr.h>
#include <Poco/AutoPtr.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/ErrorHandler.h>

#include "rpc/SocketRPCConnector.h"
#include "rpc/LegacyGatewayRPC.h"
#include "util/SecureXmlParser.h"

// These special gateway IDs are used to determine,
// how the fake-ada-server responds to the requests.
#define GATEWAYID_SUCCESS_STRING "1111222211112222"
#define GATEWAYID_SUCCESS_NUM 1111222211112222
#define GATEWAYID_FAIL_STRING "1111222233334444"
#define GATEWAYID_FAIL_NUM 1111222233334444
#define GATEWAYID_SLOW_STRING "1111111111111111"
#define GATEWAYID_SLOW_NUM 1111111111111111

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;

namespace BeeeOn {

/**
 * This class provide TCPConnection, that mimics some basic ada-server behaviour.
 * Expect XML request message from ui-server (listen, ping, delete).
 * Based on the type of request and the special gatewayIDs responds
 * with success, fail or corrupted response or just closes the socket.
 */
class FakeAdaServerConnection : public TCPServerConnection {
public:
	FakeAdaServerConnection(const StreamSocket &s) : TCPServerConnection(s)
	{
	}

	void run()
	{
		StreamSocket &sock = socket();
		AutoPtr<Document> doc = parseDocument(sock);
		Element *root = doc->documentElement();

		if (root->getAttribute("type") == "listen") {
			Element *adapterNode = root->getChildElement("adapter");
			poco_check_ptr(adapterNode);

			string gatewayID = adapterNode->getAttribute("id");
			if (gatewayID == GATEWAYID_SUCCESS_STRING)
				sendSuccessResponse(sock);
			else if (gatewayID == GATEWAYID_FAIL_STRING)
				sendFailResponse(sock);
			else if (gatewayID == GATEWAYID_SLOW_STRING)
				sendSlowResponse(sock);
			else poco_assert(false);
		}
		else if (root->getAttribute("type") == "ping") {
			Element *adapterNode = root->getChildElement("adapter");
			poco_check_ptr(adapterNode);

			string gatewayID = adapterNode->getAttribute("id");
			if (gatewayID == GATEWAYID_SUCCESS_STRING)
				sendSuccessResponse(sock);
			else if (gatewayID == GATEWAYID_FAIL_STRING)
				sendCorruptedResponse(sock);
			else poco_assert(false);
		}
		else if (root->getAttribute("type") == "delete") {
			Element *sensorNode = root->getChildElement("sensor");
			poco_check_ptr(sensorNode);

			string gatewayID = sensorNode->getAttribute("onAdapter");
			if (gatewayID == GATEWAYID_SUCCESS_STRING)
				sendSuccessResponse(sock);
			else if(gatewayID == GATEWAYID_FAIL_STRING)
				sock.close();
			else poco_assert(false);
		}
	}

private:
	void sendSuccessResponse(StreamSocket &socket)
	{
		string xml("<reply errorCode=\"0\">true</reply>");
		socket.sendBytes(xml.c_str(), xml.length());
	}

	void sendFailResponse(StreamSocket &socket)
	{
		string xml("<reply errorCode=\"1\">false</reply>");
		socket.sendBytes(xml.c_str(), xml.length());
	}

	void sendSlowResponse(StreamSocket &socket)
	{
		string xml("<reply errorCode=\"0\">true</reply>");

		for (unsigned int i = 0; i < xml.length(); ++i)
			socket.sendBytes(xml.c_str() + i, 1);
	}

	void sendCorruptedResponse(StreamSocket &socket)
	{
		string xml("<reply errorCode=\"0\">true</reply");
		socket.sendBytes(xml.c_str(), xml.length());
	}

	AutoPtr<Document> parseDocument(StreamSocket &socket)
	{
		FIFOBuffer buffer(1024);
		string xml;
		int ret = 0;

		do {
			buffer.drain(ret);
			ret = socket.receiveBytes(buffer);
			if (ret <= 0)
				throw ConnectionResetException(socket.peerAddress().toString());

			xml += string(buffer.begin(), buffer.used());
		} while (buffer.isFull());

		return m_parser.parse(xml);
	}

private:
	SecureXmlParser m_parser;
};

/**
 * This class is used to handle uncaught exceptions from the FakeAdaServer thread.
 */
class FakeAdaServerCrashErrorHandler : public Poco::ErrorHandler {
public:
	FakeAdaServerCrashErrorHandler(bool &crashed, string &errorMessage):
		m_crashed(crashed),
		m_errorMessage(errorMessage)
	{
	}

	void exception(const Poco::Exception& exc)
	{
		m_crashed = true;
		m_errorMessage = exc.displayText();
	}
	void exception(const std::exception& exc)
	{
		m_crashed = true;
		m_errorMessage = exc.what();
	}
	void exception()
	{
		m_crashed = true;
		m_errorMessage = "unknown exception";
	}
private:
	bool &m_crashed;
	string &m_errorMessage;
};

class LegacyGatewayRPCTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(LegacyGatewayRPCTest);
	CPPUNIT_TEST(testSendListenSuccess);
	CPPUNIT_TEST(testSendListenFailResponse);
	CPPUNIT_TEST(testSendListenSlowResponse);
	CPPUNIT_TEST(testUnpairDeviceSuccess);
	CPPUNIT_TEST(testUnpairDeviceMissingResponse);
	CPPUNIT_TEST(testPingGatewaySuccess);
	CPPUNIT_TEST(testPingGatewayCorruptedResponse);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testSendListenSuccess();
	void testSendListenFailResponse();
	void testSendListenSlowResponse();
	void testUnpairDeviceSuccess();
	void testUnpairDeviceMissingResponse();
	void testPingGatewaySuccess();
	void testPingGatewayCorruptedResponse();

private:
	SharedPtr<TCPServer> m_fakeAdaServer;
	bool m_fakeAdaServerCrashed = false;
	string m_fakeAdaServerErrorMessage;
	LegacyGatewayRPC *m_rpc;
	SocketRPCConnector *m_connector;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LegacyGatewayRPCTest);

void LegacyGatewayRPCTest::setUp()
{
	m_fakeAdaServerCrashed = false;
	m_fakeAdaServerErrorMessage.clear();
	FakeAdaServerCrashErrorHandler errorHandler(
		m_fakeAdaServerCrashed, m_fakeAdaServerErrorMessage);
	Poco::ErrorHandler::set(&errorHandler);

	m_fakeAdaServer.assign(new TCPServer(
		new TCPServerConnectionFactoryImpl<FakeAdaServerConnection>()));
	m_fakeAdaServer->start();

	m_connector = new SocketRPCConnector();
	m_connector->setHost("localhost");
	m_connector->setPort(m_fakeAdaServer->port());
	m_connector->setReceiveTimeout(1000000);

	m_rpc = new LegacyGatewayRPC();
	m_rpc->setRPCConnector(m_connector);
}

void LegacyGatewayRPCTest::tearDown()
{
	m_fakeAdaServer->stop();

	delete m_rpc;
	delete m_connector;
}

/**
 * SendListen with GATEWAYID_SUCCESS expects a successful response
 * from FakeAdaServer, so there should be no exception thrown.
 */
void LegacyGatewayRPCTest::testSendListenSuccess()
{
	Gateway gateway(GATEWAYID_SUCCESS_NUM);
	CPPUNIT_ASSERT_NO_THROW(m_rpc->sendListen(gateway, Timespan()));
	CPPUNIT_ASSERT_MESSAGE(
		m_fakeAdaServerErrorMessage, !m_fakeAdaServerCrashed);
}

/**
 * SendListen with GATEWAYID_FAIL expects an unsuccessful response
 * from FakeAdaServer, so there should be an IllegalStateException thrown.
 */
void LegacyGatewayRPCTest::testSendListenFailResponse()
{
	Gateway gateway(GATEWAYID_FAIL_NUM);
	CPPUNIT_ASSERT_THROW(m_rpc->sendListen(gateway, Timespan()),
			Poco::IllegalStateException);
	CPPUNIT_ASSERT_MESSAGE(
		m_fakeAdaServerErrorMessage, !m_fakeAdaServerCrashed);
}

void LegacyGatewayRPCTest::testSendListenSlowResponse()
{
	Gateway gateway(GATEWAYID_SLOW_NUM);
	m_rpc->sendListen(gateway, Timespan());
	CPPUNIT_ASSERT_MESSAGE(
		m_fakeAdaServerErrorMessage, !m_fakeAdaServerCrashed);
}

/**
 * UnparDevice with GATEWAYID_SUCCESS expects a successful response
 * from FakeAdaServer, so there should be no exception thrown.
 */
void LegacyGatewayRPCTest::testUnpairDeviceSuccess()
{
	Gateway gateway(GATEWAYID_SUCCESS_NUM);
	Device device;
	CPPUNIT_ASSERT_NO_THROW(m_rpc->unpairDevice(gateway, device));
	CPPUNIT_ASSERT_MESSAGE(
		m_fakeAdaServerErrorMessage, !m_fakeAdaServerCrashed);
}

/**
 * UnpairDevice with GATEWAYID_FAIL expects a missing response
 * from FakeAdaServer, so there should be an InvalidArgumentException thrown.
 */
void LegacyGatewayRPCTest::testUnpairDeviceMissingResponse()
{
	Gateway gateway(GATEWAYID_FAIL_NUM);
	Device device;
	CPPUNIT_ASSERT_THROW(
		m_rpc->unpairDevice(gateway, device), Poco::InvalidArgumentException);
	CPPUNIT_ASSERT_MESSAGE(
		m_fakeAdaServerErrorMessage, !m_fakeAdaServerCrashed);
}

/**
 * PingGateway with GATEWAYID_SUCCESS expects a successful response
 * from FakeAdaServer, so there should be no exception thrown.
 */
void LegacyGatewayRPCTest::testPingGatewaySuccess()
{
	Gateway gateway(GATEWAYID_SUCCESS_NUM);
	CPPUNIT_ASSERT_NO_THROW(m_rpc->pingGateway(gateway));
	CPPUNIT_ASSERT_MESSAGE(
		m_fakeAdaServerErrorMessage, !m_fakeAdaServerCrashed);
}

/**
 * PingGateway with GATEWAYID_FAIL expects a corrupted xml response
 * from FakeAdaServer, so there should be a SAXException thrown.
 */
void LegacyGatewayRPCTest::testPingGatewayCorruptedResponse()
{
	Gateway gateway(GATEWAYID_FAIL_NUM);
	CPPUNIT_ASSERT_THROW(m_rpc->pingGateway(gateway), Poco::XML::SAXException);
	CPPUNIT_ASSERT_MESSAGE(
		m_fakeAdaServerErrorMessage, !m_fakeAdaServerCrashed);
}

}
