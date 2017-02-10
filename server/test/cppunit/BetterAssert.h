#ifndef BEEEON_BETTER_ASSERT_H
#define BEEEON_BETTER_ASSERT_H

#include <cppunit/TestAssert.h>
#include <Poco/Exception.h>

CPPUNIT_NS_BEGIN

#ifdef CPPUNIT_ASSERT_THROW_MESSAGE
#undef CPPUNIT_ASSERT_THROW_MESSAGE
#endif

#define CPPUNIT_ASSERT_THROW_MESSAGE(message, expression, ExceptionType)               \
	do {                                                                           \
		bool cpputCorrectExceptionThrown_ = false;                             \
		CPPUNIT_NS::Message cpputMsg_("expected exception not thrown");        \
		cpputMsg_.addDetail(message);                                          \
		cpputMsg_.addDetail("Expected: "                                       \
			CPPUNIT_GET_PARAMETER_STRING(ExceptionType));                  \
                                                                                       \
		try {                                                                  \
			expression;                                                    \
		} catch (const ExceptionType &) {                                      \
			cpputCorrectExceptionThrown_ = true;                           \
		} catch (const Poco::Exception &e) {                                   \
			cpputMsg_.addDetail("Actual  : " +                             \
				CPPUNIT_EXTRACT_EXCEPTION_TYPE_(e,                     \
					"Poco::Exception or derived"));                \
			cpputMsg_.addDetail(std::string("What()  : ")                  \
					+ e.displayText());                            \
		} catch (const std::exception &e) {                                    \
			cpputMsg_.addDetail("Actual  : " +                             \
				CPPUNIT_EXTRACT_EXCEPTION_TYPE_(e,                     \
					"std::exception or derived"));                 \
			cpputMsg_.addDetail( std::string("What()  : ") + e.what());    \
		} catch (...) {                                                        \
			cpputMsg_.addDetail( "Actual  : unknown.");                    \
		}                                                                      \
                                                                                       \
		if (cpputCorrectExceptionThrown_)                                      \
			break;                                                         \
                                                                                       \
		CPPUNIT_NS::Asserter::fail(cpputMsg_, CPPUNIT_SOURCELINE());           \
	} while (false)


#ifdef CPPUNIT_ASSERT_NO_THROW_MESSAGE
#undef CPPUNIT_ASSERT_NO_THROW_MESSAGE
#endif

#define CPPUNIT_ASSERT_NO_THROW_MESSAGE(message, expression)                            \
	do {                                                                            \
		CPPUNIT_NS::Message cpputMsg_("unexpected exception caught");           \
		cpputMsg_.addDetail(message);                                           \
                                                                                        \
		try {                                                                   \
			expression;                                                     \
		} catch (const Poco::Exception &e) {                                    \
			cpputMsg_.addDetail("Caught: " +                                \
				CPPUNIT_EXTRACT_EXCEPTION_TYPE_(e,                      \
					"Poco::Exception or derived"));                 \
			cpputMsg_.addDetail(std::string("What(): ") + e.displayText()); \
			CPPUNIT_NS::Asserter::fail(cpputMsg_,                           \
				CPPUNIT_SOURCELINE());                                  \
		} catch (const std::exception &e) {                                     \
			cpputMsg_.addDetail("Caught: " +                                \
				CPPUNIT_EXTRACT_EXCEPTION_TYPE_(e,                      \
					"std::exception or derived"));                  \
			cpputMsg_.addDetail(std::string("What(): ") + e.what());        \
			CPPUNIT_NS::Asserter::fail(cpputMsg_,                           \
				CPPUNIT_SOURCELINE());                                  \
		} catch (...) {                                                         \
			cpputMsg_.addDetail("Caught: unknown.");                        \
			CPPUNIT_NS::Asserter::fail(cpputMsg_,                           \
				CPPUNIT_SOURCELINE());                                  \
		}                                                                       \
	} while (false)

static inline void assertEquals(
		const char *expected,
		const std::string &actual,
		SourceLine sourceLine,
		const std::string &message)
{
	if (!assertion_traits<std::string>::equal(std::string(expected), actual)) {
		Asserter::failNotEqual(
			assertion_traits<std::string>::toString(std::string(expected)),
			assertion_traits<std::string>::toString(actual),
			sourceLine,
			message);
	}
}

CPPUNIT_NS_END

#endif
