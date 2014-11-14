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

#include <string>
#include <cstdarg>

void createMsgInWithAttr(char*   msgin, string com_ver,string ses_id,string state, int nAttr , char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    std::stringstream ss;
    ss<< "<" P_COMMUNICATION " " P_VERSION "=" COM_VERSION " " P_SESSION_ID "=\"" <<ses_id <<"\" "<< P_STATE "=\"" << state<< "\" ";
    
     for (int i=0 ;i<nAttr; i++)
    {
        char* attrName = va_arg(args, char*);
        ss<< attrName;
        char* attrValue = va_arg(args, char*);
        ss<<  "=\""<< attrValue<< "\" ";
    }
    ss << " />";
    strcpy (msgin,ss.str().c_str());
    va_end(args);
}


void createMsgInWithAttr( char*   msgin, string com_ver,string ses_id,string state, std::initializer_list<string> argList )
{
    std::stringstream ss;
    ss<< "<" P_COMMUNICATION " " P_VERSION "=" COM_VERSION " " P_SESSION_ID "=\"" <<ses_id <<"\" "<< P_STATE "=\"" << state<< "\" ";
    
    for ( std::initializer_list<string>::iterator it=argList.begin(); it!=argList.end(); it++){
        ss<< *it;
        if(it!=argList.end())
            it++;
        ss<<  "=\""<< *it<< "\" ";
    }
    
    ss << " />";
    strcpy (msgin,ss.str().c_str());
}
//createMsgInWithAttr(msg, "1", "2", "st",{"x","y","z","q"});

void createMsgInWithAttributes(char*   msgin, string com_ver,string ses_id,string state){
    std::stringstream ss;
    ss<< "<" P_COMMUNICATION " " P_VERSION "=" COM_VERSION " " P_SESSION_ID "=\"" <<ses_id <<"\" "<< P_STATE "=\"" << state<< "\" " <<
            " />";
    
    strcpy (msgin,ss.str().c_str());
}void createMsgInWithAttributes(char*   msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value){
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
void createMsgInWithAttributes( char*  msgin, string com_ver,string ses_id,string state,string attr1name, string attr1value,string attr2name, string attr2value,string attr3name, string attr3value,string attr4name, string attr4value,
        string attr5name,string attr5value,string attr6name, string attr6value,string attr7name, string attr7value){
    
    std::stringstream ss;
    ss<< "<" P_COMMUNICATION " " P_VERSION "=" COM_VERSION " " P_SESSION_ID "=\"" <<ses_id <<"\" "<< P_STATE "=\"" << state<< "\" " <<
            attr1name<< "=\""<< attr1value<< "\" "<<attr2name<< "=\""<< attr2value<< "\" "<< attr3name<<"=\""<< attr3value<< "\" "<< attr4name<< "=\""<< attr4value<<"\" "<<
            attr5name<< "=\""<< attr5value<< "\" "<<attr6name<< "=\""<< attr6value<< "\" "<< attr7name<<"=\""<< attr7value <<"\" "<<
            " />";
    
    strcpy (msgin,ss.str().c_str());
}
