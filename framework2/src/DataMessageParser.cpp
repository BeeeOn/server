/* 
 * File:   MessageParser.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "DataMessageParser.h"

#include <iostream>
#include <utility>

#include "pugixml.hpp"
#include "pugiconfig.hpp"

DataMessageParser::DataMessageParser() {
}

DataMessageParser::~DataMessageParser() {
}

DataMessage DataMessageParser::parseMessage(std::string received_data)
{

    DataMessage data_message;
    
    pugi::xml_document doc;
    pugi::xml_node current_node;
    pugi::xml_attribute current_attribute;
    
    pugi::xml_parse_result result = doc.load_buffer(received_data.c_str(), received_data.size());
    
    if (!result) {
        std::cerr << "Could not parse received data message." << std::endl;
        std::cerr << "Error description: " << result.description() << std::endl;
        std::cerr << "Error offset: " << result.offset << " (error at [..." << (received_data.c_str() + result.offset) << "]" << std::endl;
        
        throw std::runtime_error("Parsing of data message was not successful.");
    }

    /*
    <adapter_server adapter_id="0x57d3e01695f35" fw_version="v1.5" protocol_version="1.1" state="data" time="1448964949">
        <device device_id="0x00" euid="0xea000011" name="own_name">
            <values count="6">
                <value module_id="0x00">25.19</value>
                <value module_id="0x01">-0.01</value>
                <value module_id="0x02">35.61</value>
                <value module_id="0x03">68.00</value>
                <value module_id="0x05" status="unavailable"></value>
                <value module_id="0x04">100.00</value>
            </values>
        </device>
    </adapter_server>
    */
    
    current_node = doc.child("adapter_server");
    data_message.gateway_id = current_node.attribute("adapter_id").as_ullong();
    data_message.time = current_node.attribute("time").as_uint();
    
    current_node = current_node.child("device");
    data_message.device_euid = current_node.attribute("euid").as_ullong();
    
    current_node = current_node.child("values");
    for (pugi::xml_node value : current_node.children("value")) {
        
        MODULE_STATUS status;
        if (value.attribute("status") == NULL) {
            // If there is no attribute "status", module is available.
            status = MODULE_STATUS::AVAILABLE;
        }
        else {
            // If attribute "status" is present, module is unavailable.
            status = MODULE_STATUS::UNAVAILABLE;
        }
        data_message.modules.emplace(value.attribute("module_id").as_int(), std::make_pair(status, value.text().as_float()));
    }
    
    return data_message;
}
