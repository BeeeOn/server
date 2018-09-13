#pragma once

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

	/**
	 * @brief It always fallbacks to single() passing only
	 * the first value of the vector.
	 */
	void multiple(const std::vector<ValueAt> &v) override;
	void frequency(const ValueAt &v, size_t count) override;
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
