#include "IMsgIn.h"
#include "save_custom_writer.h"
using namespace std;
const int IMsgIn::MAJOR_VERSION = 2;
const int IMsgIn::MINOR_VERSION = 5;
const string IMsgIn::VERSION = to_string((long long int)MAJOR_VERSION) + "." + to_string((long long int)MINOR_VERSION);


IMsgIn::IMsgIn(pugi::xml_document* doc)
{
    //std::cout<<"creating msgIn..."<<flush;
    _doc = doc;
    
    _outputMainNode = _outputDoc.append_child();
    _outputMainNode.set_name(proto::communicationNode);
    
    
    _token = _doc->child(proto::communicationNode).attribute(proto::sessionIdAttr).value();
    _state = _doc->child(proto::communicationNode).attribute(proto::stateAttr).value();
    
   /* try{
        _IHAtoken = stoi(IHAtoken);
        if(_IHAtoken < 0 )
            _IHAtoken = -1;
    }catch(...){
        _IHAtoken = -1;
    }*/
        
}
IMsgIn::~IMsgIn(void)
{
    delete _doc;
    //free(_msg);
}

t_version IMsgIn::parseProtocolVersion(std::string version) {
    t_version ver;
    
    istringstream f(version);
    string s;
    
    try{
        getline(f, s, '.');
        ver.majorVersion = atoi(s.c_str());

        getline(f, s, '.');
        ver.minorVersion = atoi(s.c_str());
    }catch(...){
        Logger::error() <<"version parsing: "<< version <<endl;
        ver.majorVersion = -1;
        ver.minorVersion = -1;
    }    
    return ver;
}


bool IMsgIn::checkProtocolVersion(){
    string protocolVersion = _doc->child(proto::communicationNode).attribute(proto::versionAttr).value();
    
    t_version ver = parseProtocolVersion(protocolVersion);
    
    Logger::debug3() << "major ver.: " << ver.majorVersion << " minor ver.: " << ver.minorVersion <<endl; 
    
    if(ver.majorVersion != MAJOR_VERSION ) {
        Logger::error() <<"BAD protocol v. - expected: "<< VERSION<<" actual "<<protocolVersion<<endl;
        return false;
    }else{
        return true;
    }
}





string IMsgIn::envelopeResponseSetAttributes(string state, string response, string attributes)
{ 
        std::stringstream ss;
        //std::cout<<"envelope Msg "<<getState()<<std::endl;
        ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><com " <<
                        proto::versionAttr  << "="<<"\""<<VERSION<<"\" "<<
                        //proto::sessionIdAttr "=\"" << this->_IHAtoken << "\" "<<
                        proto::stateAttr <<"=\"" << state <<  "\""<<
                        " " << attributes<<
                        ">" 
                        << response << 
                "</com>" << std::endl;
        
        return ss.str();
}
string IMsgIn::envelopeResponse(string state)
{ 
    return envelopeResponseSetAttributes( state, "", "");
}

string IMsgIn::envelopeResponse(string state, string response)
{ 
    return envelopeResponseSetAttributes( state, response, "");
}

string IMsgIn::envelopeResponse(string state, string response, int errcode)
{        
    string additionalAttributes = (string)proto::errorCodeAttr+"=\""+to_string((long long int)errcode)+"\"";
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}

string IMsgIn::envelopeResponseWithRole(string state, string response, string role)
{        
    string additionalAttributes = "";
    if(role != "")
        additionalAttributes = (string)proto::userRoleAttr+"=\"" + role +"\"" ;
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}


string IMsgIn::envelopeResponseWithAttributes(string state, string attributes) {
    return envelopeResponseSetAttributes(state, "", attributes);
}

string IMsgIn::makeXMLattribute(string attr, string value) {
    return attr+"=\""+value+"\"";
}

void IMsgIn::makeCommunicationHeader(std::string responseState) {
    pugi::xml_node declaration = _outputDoc.prepend_child(pugi::node_declaration);
    declaration.append_attribute("version") = "1.0";
    declaration.append_attribute("encoding") = "UTF-8";
    
    _outputMainNode.prepend_attribute(proto::stateAttr) = responseState.c_str();
    _outputMainNode.prepend_attribute(proto::versionAttr) = IMsgIn::VERSION.c_str();
    _outputMainNode.append_child(pugi::node_pcdata).set_value("");
}


std::string IMsgIn::getXMLreply(std::string responseState) {
    
    makeCommunicationHeader(responseState);
    //_outputDoc.print(std::cout);
    
    return node_to_string(_outputDoc);
   /* std:: ostringstream ss; 
    _outputDoc.save (ss); 
    std:: string s = ss.str (); 
    return s;*/
}

std::string IMsgIn::getNegativeXMLReply(int errorCode) {
        _outputMainNode.append_attribute(proto::errorCodeAttr) = errorCode;
        return getXMLreply(proto::replyFalse);
}

std::string IMsgIn::getNegativeXMLReply(int errorCode, const char* errorInfo) {
        _outputMainNode.append_attribute(proto::errorCodeAttr) = errorCode;
        _outputMainNode.append_child(pugi::node_pcdata).set_value(errorInfo);
        return getXMLreply(proto::replyFalse);
}

bool IMsgIn::isRoleValid(std::string role) {
    return role == proto::roleGuestAttr || role == proto::roleUserAttr || role ==proto::roleAdminAttr || role == proto::roleSuperuserAttr;
}
