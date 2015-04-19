#include "IMsgIn.h"
#include "save_custom_writer.h"
using namespace std;
const string IMsgIn::VERSION="2.4";


IMsgIn::IMsgIn(char* msg, pugi::xml_document* doc)
{
    //std::cout<<"creating msgIn..."<<flush;
    _msg = msg;
    _doc = doc;
    
    _outputMainNode = _outputDoc.append_child();
    _outputMainNode.set_name(P_COMMUNICATION);
    
    
    _token = _doc->child(P_COMMUNICATION).attribute(P_SESSION_ID).value();
    _state = _doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
   /* try{
        _IHAtoken = stoi(IHAtoken);
        if(_IHAtoken < 0 )
            _IHAtoken = -1;
    }catch(...){
        _IHAtoken = -1;
    }*/
    
    _adapterId = _doc->child(P_COMMUNICATION).attribute(P_ADAPTER_ID).value();
        
}
IMsgIn::~IMsgIn(void)
{
    delete _doc;
    //free(_msg);
}

bool IMsgIn::checkProtocolVersion(){
    string protocolVersion = _doc->child(P_COMMUNICATION).attribute(P_VERSION).value();
    if(protocolVersion == VERSION) {
        return true;
    }else{
        Logger::getInstance(Logger::ERROR) <<"BAD protocol v. - expected: "<< VERSION<<" actual "<<protocolVersion<<endl;
        return false;
    }
}





string IMsgIn::envelopeResponseSetAttributes(string state, string response, string attributes)
{ 
        std::stringstream ss;
        //std::cout<<"envelope Msg "<<getState()<<std::endl;
        ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><com "
                        P_VERSION "="<<"\""<<VERSION<<"\" "<<
                        //P_SESSION_ID "=\"" << this->_IHAtoken << "\" "<<
                        P_STATE "=\"" << state <<  "\""<<
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
    string additionalAttributes = (string)P_ERRCODE+"=\""+to_string((long long int)errcode)+"\"";
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}

string IMsgIn::envelopeResponseWithRole(string state, string response, string role)
{        
    string additionalAttributes = "";
    if(role != "")
        additionalAttributes = (string)P_ROLE+"=\"" + role +"\"" ;
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}

string IMsgIn::envelopeResponseWithAdapterId(string state, string response)
{        
    string additionalAttributes = "";
    additionalAttributes = (string)P_ADAPTER_ID+"=\"" + _adapterId +"\"" ;
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}

string IMsgIn::envelopeResponseWithAdapterId(string state, string response, string adapterId)
{        
    string additionalAttributes = "";
    if(adapterId != "")
        additionalAttributes = (string)P_ADAPTER_ID+"=\"" + adapterId +"\"" ;
    return envelopeResponseSetAttributes( state, response, additionalAttributes);
}

string IMsgIn::envelopeResponseWithAttributes(string state, string attributes) {
    return envelopeResponseSetAttributes(state, "", attributes);
}

string IMsgIn::makeXMLattribute(string attr, string value) {
    return attr+"=\""+value+"\"";
}

std::string IMsgIn::genOutputXMLwithVersionAndState(std::string responseState) {
    
        
    
    _outputMainNode.prepend_attribute(P_STATE) = responseState.c_str();
    _outputMainNode.prepend_attribute(P_VERSION) = IMsgIn::VERSION.c_str();
    _outputMainNode.append_child(pugi::node_pcdata).set_value("");
    //_outputDoc.print(std::cout);
    
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + node_to_string(_outputDoc);
   /* std:: ostringstream ss; 
_outputDoc.save (ss); 
std:: string s = ss.str (); 
return s;*/
}
