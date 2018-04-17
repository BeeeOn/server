#ifndef BEEEON_XML_VALUE_CONSUMER_H
#define BEEEON_XML_VALUE_CONSUMER_H

#include "service/ValueConsumer.h"

namespace Poco {
namespace XML {

class XMLWriter;

}
}

namespace BeeeOn {
namespace XmlUI {

class XmlValueConsumer : public BeeeOn::ValueConsumer {
public:
	XmlValueConsumer(Poco::XML::XMLWriter &writer);

	bool hasBegin() const;

	void begin(const TypeInfo &info) override;
	void single(const ValueAt &v) override;
	void end() override;

protected:
	const TypeInfo &info() const;

private:
	Poco::XML::XMLWriter &m_writer;
	const TypeInfo *m_info;
	bool m_hasBegin;
};

}
}

#endif
