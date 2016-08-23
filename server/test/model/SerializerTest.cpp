#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

#include "model/JSONSerializer.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

namespace BeeeOn {

class SerializerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SerializerTest);
	CPPUNIT_TEST(testSerializeObject);
	CPPUNIT_TEST(testDeserializeObject);
	CPPUNIT_TEST(testDeserializeObjectFromStream);
	CPPUNIT_TEST(testSerializeArray);
	CPPUNIT_TEST(testDeserializeArray);
	CPPUNIT_TEST(testDeserializeMemberArray);
	CPPUNIT_TEST_SUITE_END();

public:
	SerializerTest()
	{
	}

	void setUp();
	void tearDown();
	void testSerializeObject();
	void testDeserializeObject();
	void testDeserializeObjectFromStream();
	void testSerializeArray();
	void testDeserializeArray();
	void testDeserializeMemberArray();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SerializerTest);

/**
 * The SerializableEntry is an example model object to be
 * serialized.
 */
class SerializableEntry {
public:
	SerializableEntry()
	{
	}

	SerializableEntry(const string &name, int size):
		m_name(name), m_size(size)
	{
	}

	template <typename Serializer>
	void serialize(Serializer &s)
	{
		s.push("name", m_name);
		s.push("size", m_size);
	}

	template <typename Serializer>
	void deserialize(Serializer &s)
	{
		s.get("name", m_name);
		s.get("size", m_size);
	}

	string m_name;
	int m_size;
};

void SerializerTest::setUp()
{
}

void SerializerTest::tearDown()
{
}

void SerializerTest::testSerializeObject()
{
	JSONObjectSerializer serializer;
	SerializableEntry e("John Doe", 150);

	e.serialize(serializer);

	CPPUNIT_ASSERT(serializer.toString()
		.compare("{\"name\":\"John Doe\",\"size\":150}") == 0);
}

void SerializerTest::testDeserializeObject()
{
	JSONObjectSerializer serializer(
			"{\"name\":\"Michael God\",\"size\":250}");

	SerializableEntry e;
	e.deserialize(serializer);

	CPPUNIT_ASSERT(e.m_name.compare("Michael God") == 0);
	CPPUNIT_ASSERT(e.m_size == 250);
}

void SerializerTest::testDeserializeObjectFromStream()
{
	istringstream s("{\"name\":\"Robbie Rock\",\"size\":110}");
	JSONObjectSerializer serializer(s);

	SerializableEntry e;
	e.deserialize(serializer);

	CPPUNIT_ASSERT(e.m_name.compare("Robbie Rock") == 0);
	CPPUNIT_ASSERT(e.m_size == 110);
}

void SerializerTest::testSerializeArray()
{
	JSONArraySerializer serializer;
	SerializableEntry e1("John Doe", 140);
	SerializableEntry e2("Jack Sparrow", 1000);
	SerializableEntry e3("Jennifer Fantastic", 55);

	JSONObjectSerializer e1Serializer = serializer.startObject();
	e1.serialize(e1Serializer);
	serializer.endObject(e1Serializer);

	JSONObjectSerializer e2Serializer = serializer.startObject();
	e2.serialize(e2Serializer);
	serializer.endObject(e2Serializer);

	JSONObjectSerializer e3Serializer = serializer.startObject();
	e3.serialize(e3Serializer);
	serializer.endObject(e3Serializer);

	CPPUNIT_ASSERT(serializer.size() == 3);
	CPPUNIT_ASSERT(serializer.toString().compare(
		"[{\"name\":\"John Doe\",\"size\":140},"
		"{\"name\":\"Jack Sparrow\",\"size\":1000},"
		"{\"name\":\"Jennifer Fantastic\",\"size\":55}]") == 0);
}

void SerializerTest::testDeserializeArray()
{
	JSONArraySerializer serializer(
		"[{\"name\":\"Michael God\",\"size\":250},"
		"{\"name\":\"Mickey Mouse\",\"size\":99}]");

	CPPUNIT_ASSERT(serializer.size() == 2);

	for (unsigned int i = 0; i < serializer.size(); ++i)
		CPPUNIT_ASSERT(serializer.isObject(i));

	JSONObjectSerializer e1Serializer = serializer.getObject(0);
	SerializableEntry e1;
	e1.deserialize(e1Serializer);

	CPPUNIT_ASSERT(e1.m_name.compare("Michael God") == 0);
	CPPUNIT_ASSERT(e1.m_size == 250);

	JSONObjectSerializer e2Serializer = serializer.getObject(1);
	SerializableEntry e2;
	e2.deserialize(e2Serializer);

	CPPUNIT_ASSERT(e2.m_name.compare("Mickey Mouse") == 0);
	CPPUNIT_ASSERT(e2.m_size == 99);
}

void SerializerTest::testDeserializeMemberArray()
{
	JSONObjectSerializer serializer(
	"{"
		"\"id\":42,"
		"\"config\":["
			"{\"name\":\"Michael God\",\"size\":250},"
			"{\"name\":\"Mickey Mouse\",\"size\":99}"
		"]"
	"}");

	JSONArraySerializer arraySerializer = serializer.getArray("config");
	CPPUNIT_ASSERT(arraySerializer.size() == 2);

	for (unsigned int i = 0; i < arraySerializer.size(); ++i)
		CPPUNIT_ASSERT(arraySerializer.isObject(i));
}

}
