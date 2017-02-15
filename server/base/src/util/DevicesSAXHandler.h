#ifndef BEEEON_DEVICES_SAX_HANDLER_H
#define BEEEON_DEVICES_SAX_HANDLER_H

#include <set>

#include "model/ModuleInfo.h"
#include "model/DeviceInfo.h"
#include "util/SAXHelper.h"

namespace BeeeOn {

class DevicesSAXHandler : public SAXHelper {
public:
	DevicesSAXHandler();
	~DevicesSAXHandler();

	std::set<DeviceInfo>::const_iterator begin() const
	{
		return m_result.begin();
	}

	std::set<DeviceInfo>::const_iterator end() const
	{
		return m_result.end();
	}

protected:
	bool expectElement(const SAXElement &element) const override;
	bool expectContent(int length) const override;

	void startElement(const SAXElement &element,
			const Poco::XML::Attributes &attrList) override;
	void endElement(const SAXElement &element) override;

	void preprocessContent(const Poco::XML::XMLChar c[],
			int &start, int &length) override;

private:
	std::set<Poco::XML::XMLString> m_deviceExpect;
	std::set<Poco::XML::XMLString> m_modulesExpect;
	std::set<Poco::XML::XMLString> m_sensorExpect;
	std::set<Poco::XML::XMLString> m_actuatorExpect;
	std::set<Poco::XML::XMLString> m_refreshExpect;
	std::set<Poco::XML::XMLString> m_constraintsExpect;
	std::set<Poco::XML::XMLString> m_contentElements;
	std::set<DeviceInfo> m_result;
	DeviceInfo m_device;
	ModuleInfo m_module;
};

}

#endif
