#include "IRequestHandler.h"
#include "save_custom_writer.h"
using namespace std;
const int IRequestHandler::MAJOR_VERSION = 1;
const int IRequestHandler::MINOR_VERSION = 0;
const int IRequestHandler::PATCH_VERSION = 0;
const string IRequestHandler::VERSION = to_string((long long int)MAJOR_VERSION) 
        + "." + to_string((long long int)MINOR_VERSION) 
        + "." + to_string((long long int)PATCH_VERSION);

IRequestHandler::IRequestHandler(shared_ptr<pugi::xml_document> doc)
{
    _doc = doc;
    
    _outputMainNode = _outputDoc.append_child();
    _outputMainNode.set_name(proto::responseNode);

    _token = _doc->child(proto::requestNode).attribute(proto::sessionIdAttr).value();
    _state = _doc->child(proto::requestNode).attribute(proto::typeAttr).value();
    _namespace = _doc->child(proto::requestNode).attribute(proto::namespaceAttr).value();
}

IRequestHandler::~IRequestHandler(void)
{
}

t_version IRequestHandler::parseProtocolVersion(std::string version) 
{
    t_version ver;
    
    istringstream f(version);
    string s;
    
    try{
        getline(f, s, '.');
        ver.majorVersion = atoi(s.c_str());

        getline(f, s, '.');
        ver.minorVersion = atoi(s.c_str());
        
        getline(f, s, '.');
        ver.patchVersion = atoi(s.c_str());
    }catch(...){
        Logger::error() <<"version parsing: "<< version <<endl;
        ver.majorVersion = -1;
        ver.minorVersion = -1;
        ver.patchVersion = -1;
    }    
    return ver;
}

bool IRequestHandler::checkProtocolVersion()
{
    string protocolVersion = _doc->child(proto::requestNode).attribute(proto::versionAttr).value();
    
    t_version ver = parseProtocolVersion(protocolVersion);
    
    Logger::debug3() << "major ver.: " << ver.majorVersion << " minor ver.: " << ver.minorVersion <<endl; 
    
    if(ver.majorVersion != MAJOR_VERSION ) {
        Logger::error() <<"BAD protocol v. - expected: "<< VERSION<<" actual "<<protocolVersion<<endl;
        return false;
    }else{
        return true;
    }
}

std::string IRequestHandler::getDataXMLreply(std::string responseState, std::string data)
{
    std::stringstream ss;
        ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><" << proto::responseNode <<" " <<
                        proto::versionAttr  << "="<<"\""<<VERSION<<"\" "<<
                        //proto::sessionIdAttr "=\"" << this->_IHAtoken << "\" "<<
                        proto::namespaceAttr <<"=\"" << _namespace <<  "\" "<<
                        proto::typeAttr <<"=\"" << this->_state <<  "\""<<
                        " " << makeXMLattribute(proto::resultAttr, proto::replyData)<<
                        ">" 
                        << data << 
                "</" << proto::responseNode <<">" << std::endl;
        
    return ss.str();
}

string IRequestHandler::makeXMLattribute(string attr, string value) {
    return attr+"=\""+value+"\"";
}

void IRequestHandler::makeCommunicationHeader(std::string responseState) 
{
    pugi::xml_node declaration = _outputDoc.prepend_child(pugi::node_declaration);
    declaration.append_attribute("version") = "1.0";
    declaration.append_attribute("encoding") = "UTF-8";
    
    if(responseState == proto::replyTrue || responseState == proto::replyFalse)
        _outputMainNode.prepend_attribute(proto::resultAttr) = responseState.c_str();
    else
        _outputMainNode.prepend_attribute(proto::resultAttr) = proto::replyData;
    
    _outputMainNode.prepend_attribute(proto::typeAttr) = this->_state.c_str();
    _outputMainNode.prepend_attribute(proto::namespaceAttr) = _namespace.c_str();
    _outputMainNode.prepend_attribute(proto::versionAttr) = IRequestHandler::VERSION.c_str();
    _outputMainNode.append_child(pugi::node_pcdata).set_value("");
}


std::string IRequestHandler::getXMLreply(std::string responseState) 
{
    makeCommunicationHeader(responseState);
    return node_to_string(_outputDoc);
}

std::string IRequestHandler::getPositiveXMLReply() 
{
    return getXMLreply(proto::replyTrue);
}

std::string IRequestHandler::getNegativeXMLReply(int errorCode) 
{
    _outputMainNode.append_attribute(proto::errorCodeAttr) = errorCode;
    return getXMLreply(proto::replyFalse);
}

std::string IRequestHandler::getNegativeXMLReply(int errorCode, const char* errorInfo) 
{
    _outputMainNode.append_attribute(proto::errorCodeAttr) = errorCode;
    _outputMainNode.append_child(pugi::node_pcdata).set_value(errorInfo);
    return getXMLreply(proto::replyFalse);
}

bool IRequestHandler::isRoleValid(std::string role) 
{
    return role == proto::roleGuestAttr || role == proto::roleUserAttr || role == proto::roleAdminAttr || role == proto::roleOwnerAttr;
}
