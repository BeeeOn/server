/*
 * messageParsers.h
 *
 *  Created on: Feb 18, 2015
 *      Author: tuso
 */

#ifndef MESSAGEPARSERS_H_
#define MESSAGEPARSERS_H_

#include "structures.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include <arpa/inet.h> //kniznica pre internetove operacie
#include <netdb.h> //kniznica pre sietove operacie
#include <cstdlib>  //kniznica pre standarnde funkcie a dynamicku pamat
#include <netinet/in.h>  //kniznica pre sietovy protokol
#include "loger.h"
#include <string>  //c++ znakove retazce
#include <cstring>
#include <bitset>

class MessageParser
{
	private:
		const std::string _Name="MessageParser";
	protected:
		tmessage* _message;			/**< spracovana sprava*/
		int _AnswerSize;			/**< velkost odpovede*/
		pugi::xml_node *_adapter,_device;
		Loger *_log;
	public:
		MessageParser() {};
		virtual bool ParseMessage(pugi::xml_node *adapter,float FM,float CP) = 0;
		virtual std::string CreateAnswer(int value) = 0;
		void setAdapterSocket(int Soc){this->_message->socket=Soc;};
		tmessage* ReturnMessage();
		int AnswerSize() {return (this->_AnswerSize);};
		virtual  ~MessageParser() { };
		virtual void GetTimeStamp()=0;
		virtual void GetID()=0;
		virtual void GetFM()=0;
		virtual void GetCP()=0;
		virtual void GetDeviceID()=0;
		virtual void GetBattery()= 0;
		virtual void GetSignal()= 0;
		virtual bool GetValues()= 0;
		virtual void GetValuesCount()= 0;
		virtual void GetState() = 0;
};

/** @brief Class ProtocolV1MessageParser

    Trieda sluziaca na spracovanie spravy od adaptera vo verzii protokolu 1
    */

class ProtocolV1MessageParser :public MessageParser
{
	private:
		const std::string _Name="ProtocolV1MessageParser";
	protected:
		virtual void GetTimeStamp();
		virtual void GetID();
		virtual void GetFM();
		virtual void GetCP();
		virtual void GetDeviceID();
		virtual void GetBattery();
		virtual void GetSignal();
		virtual bool GetValues();
		virtual void GetValuesCount();
		virtual void GetState();

	public:
		ProtocolV1MessageParser(Loger *L) ;
		virtual bool ParseMessage(pugi::xml_node *adapter,float FM,float CP);
		virtual std::string CreateAnswer(int value);
		~ProtocolV1MessageParser();
};

class UIServerMessageParser
{
	private:
		tmessage *_Message;
		std::string _toParse;
		Loger* _log;
		pugi::xml_node request;
		pugi::xml_node subject;
		const std::string _Name = "UIServerMessageParser";
	public:
		bool ParseMessage(std::string MSG);
		bool GetAdapterID();
		bool GetSubjectID();
		bool GetState();
		bool GetValue();
		bool GetOffsetType();
		UIServerMessageParser(Loger *l);
		~UIServerMessageParser();
		tmessage* ReturnMessage(){return (_Message);};

};


#endif /* MESSAGEPARSERS_H_ */
