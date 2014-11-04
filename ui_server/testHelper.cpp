#include "testHelper.h"
string getState(pugi::xml_document* doc){
    return doc->child(P_COMMUNICATION).attribute(P_STATE).value();
}
int getSessionId(pugi::xml_document* doc){
    return doc->child(P_COMMUNICATION).attribute(P_SESSION_ID).as_int(-1);
}
int getErrCode(pugi::xml_document* doc){
    return doc->child(P_COMMUNICATION).attribute(P_ERRCODE).as_int(-1);
}
void createMsgInWithAttributes(char*   msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value){
    std::stringstream ss;
    ss<< "<" P_COMMUNICATION " " P_VERSION "=" COM_VERSION " " P_SESSION_ID "=\"" <<ses_id <<"\" "<< P_STATE "=\"" << state<< "\" " <<
            attr1name<< "=\""<< attr1value<< "\" "<<
            " />";
    
    strcpy (msgin,ss.str().c_str());
}

void createMsgInWithAttributes(char*   msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value,string attr2name, string attr2value,string attr3name, string attr3value){
    std::stringstream ss;
    ss<< "<" P_COMMUNICATION " " P_VERSION "=" COM_VERSION " " P_SESSION_ID "=\"" <<ses_id <<"\" "<< P_STATE "=\"" << state<< "\" " <<
            attr1name<< "=\""<< attr1value<< "\" "<<attr2name<< "=\""<< attr2value<< "\" "<< attr3name<<"=\""<< attr3value<< "\" "<<
            " />";
    
    strcpy (msgin,ss.str().c_str());
}

void createMsgInWithAttributes( char*  msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value,string attr2name, string attr2value,string attr3name, string attr3value,string attr4name, string attr4value){
    
    std::stringstream ss;
    ss<< "<" P_COMMUNICATION " " P_VERSION "=" COM_VERSION " " P_SESSION_ID "=\"" <<ses_id <<"\" "<< P_STATE "=\"" << state<< "\" " <<
            attr1name<< "=\""<< attr1value<< "\" "<<attr2name<< "=\""<< attr2value<< "\" "<< attr3name<<"=\""<< attr3value<< "\" "<< attr4name<< "=\""<< attr4value<<"\" "<<
            " />";
    
    strcpy (msgin,ss.str().c_str());
}
