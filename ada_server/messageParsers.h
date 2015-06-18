/**
 * @file messageParsers.h
 * 
 * @brief definition of MessageParser UIServerMessageParser ProtocolV1MessageParser classes
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#ifndef MESSAGEPARSERS_H_
#define MESSAGEPARSERS_H_

#include "structures.h"
#include "pugi/pugixml.hpp"
#include "pugi/pugiconfig.hpp"
#include <arpa/inet.h> 
#include <netdb.h> 
#include <cstdlib>  
#include <netinet/in.h>  
#include "loger.h"
#include <string> 
#include <cstring>
#include <bitset>


/** @Class MessageParser
 *  @brief Abstract class which defines method for all parsers which will inherite from it
 */

class MessageParser
{
	private:
		const std::string _Name="MessageParser"; /**< name of class*/
	protected:
		tmessage* _message;			/**< parsed message*/
		int _AnswerSize;			/**< size of answer*/
		pugi::xml_node *_adapter,_device; /**< xml nodes of device and adapter */
		Loger *_log; /**<  Logger used to log */
	public:
		/**Constructor
		 */
		MessageParser() {};
		/**Pure virtual method to parse message
		 * @param adapter xml_node of adapter element in message
		 * @param CP version of communication protocol
		 * @param FM version of firmware
        */
		virtual bool ParseMessage(pugi::xml_node *adapter,float FM,float CP) = 0;
		/**Pure virtual method to create answer
		 * @param value time to next wakeup
		*/
		virtual std::string CreateAnswer(int value) = 0;
		/**Method to set socket in message
		 * @param Soc int representation of sockett
		*/
		void setAdapterSocket(int Soc){this->_message->socket=Soc;};
		/**Method to return pointer to parsed message
		 * @return pointer to parsed message
		*/
		tmessage* ReturnMessage();
		/**Method to return size of answer
		 * @return int representing number of bytes
		*/
		int AnswerSize() {return (this->_AnswerSize);};
		/** Destructor
		 */
		virtual  ~MessageParser() { };
		/**Pure virtual method to obtain time stamp
		*/
		virtual void GetTimeStamp()=0;
		/**Pure virtual method to obtain id
				*/
		virtual void GetID()=0;
		/**Pure virtual method to obtain firmware version
				*/
		virtual void GetFM()=0;
		/**Pure virtual method to obtain protocol version
				*/
		virtual void GetCP()=0;
		/**Pure virtual method to obtain device id
				*/
		virtual void GetDeviceID()=0;
		/**Pure virtual method to obtain battery value
				*/
		virtual void GetBattery()= 0;
		/**Pure virtual method to obtain signal value
				*/
		virtual void GetSignal()= 0;

		/**Pure virtual method to obtain values
		 * @return bool true/false on success/failure
				*/
		virtual bool GetValues()= 0;
		/**Pure virtual method to obtain values count
				*/
		virtual void GetValuesCount()= 0;
		/**Pure virtual method to obtain type of message
				*/
		virtual void GetState() = 0;
};

/** @Class ProtocolV1MessageParser

    Class representing parser of message in protocol version 1
    */

class ProtocolV1MessageParser :public MessageParser
{
	private:
		const std::string _Name="ProtocolV1MessageParser";
	protected:
		/**Virtual method to obtain time stamp
				*/
		virtual void GetTimeStamp();
		/**Virtual method to obtain firmware version
						*/
		virtual void GetID();
		/**Virtual method to obtain firmware version
				*/
		virtual void GetFM();
		/**Virtual method to obtain protocol version
				*/
		virtual void GetCP();
		/**Virtual method to obtain device id
						*/
		virtual void GetDeviceID();
		/**Virtual method to obtain battery value
						*/
		virtual void GetBattery();
		/**Virtual method to obtain signal value
						*/
		virtual void GetSignal();
		/**Virtual method to obtain values
		 * @return bool true/false on success/failure
				*/
		virtual bool GetValues();
		/**Virtual method to obtain values count
				*/
		virtual void GetValuesCount();
		/**Virtual method to obtain type of message
						*/
		virtual void GetState();

	public:
		/**Constructor
		 * @param l to use as loger
		*/
		ProtocolV1MessageParser(Loger *L) ;
		/**Virtual method to parse message
		 * @param adapter xml_node of adapter element in message
		 * @param CP version of communication protocol
		 * @param FM version of firmware
		*/
		virtual bool ParseMessage(pugi::xml_node *adapter,float FM,float CP);
		/**Virtual method to create answer
		 * @param value time to next wakeup
		*/
		virtual std::string CreateAnswer(int value);
		/** Destructor
		 */
		~ProtocolV1MessageParser();
};

/** @Class UIServerMessageParser
 *  Class responsible for parsing of ui_server messages
 */

class UIServerMessageParser
{
	private:
		tmessage *_Message; /**< parsed message*/
		std::string _toParse; /**< string representation of message to parse*/
		Loger* _log; /**< logger*/
		pugi::xml_node request; /**< xml node representing request element*/
		pugi::xml_node subject; /**< xml node representing subject of request*/
		const std::string _Name = "UIServerMessageParser";
	public:
		/**Method to parse message
		 * @param MSG string containing message to parse
		 * @return bool true/false on success/failure
		 */
		bool ParseMessage(std::string MSG);
		/**Method to obtain adapter ID
		 * @return bool true/false on success/failure
		 */
		bool GetAdapterID();
		/**Method to obtain subject ID
		 * @return bool true/false on success/failure
		 */
		bool GetSubjectID();
		/**Method to obtain message type ID
		 * @return bool true/false on success/failure
		 */
		bool GetState();
		/**Method to obtain value
		 * @return bool true/false on success/failure
		 */
		bool GetValue();
		/**Method to obtain type and offset
		 * @return bool true/false on success/failure
		 */
		bool GetOffsetType();
		/**Constructor
		 * @return bool true/false on success/failure
		 */
		UIServerMessageParser(Loger *l);
		/** Destructor
		 */
		~UIServerMessageParser();
		/**Method return parsed message
				 * @return pointer to tmessage structure
				 */
		tmessage* ReturnMessage(){return (_Message);};

};


#endif /* MESSAGEPARSERS_H_ */
