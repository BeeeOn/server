/**
 * @file structures.h
 * 
 * @brief definition helping structures of program
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "../lib/pugixml.hpp"
#include "../lib/pugiconfig.hpp"
#include <string>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <exception>

#ifndef STRUCTURES_H_
#define STRUCTURES_H_


class CertificateLoadException: public std::exception
{
  virtual const char* what() const throw()
  {
    return ("Error during certificates loading");
  }
};

class WorkerPoolCreateException: public std::exception
{
  virtual const char* what() const throw()
  {
    return ("Error during workerpool creation");
  }
};

/** @enum valueTypes
 *  @brief Enumerator for types of values (table of types version 1.0)
 */

//typedef enum valueTypes
//{
	//ENUM = 0x01, /**< enumeration type*/
	//TEMPERATURE = 0x02, /**< temperature */
	//HUMIDITY = 0x03, /**< humidity */
	//PRESSURE = 0x04, /**< pressure */
	//LIGHT = 0x05, /**< light */
	//NOISE = 0x06, /**< noise */
	//CO2 = 0x07, /**< CO2 */
	//BATTERY = 0x08, /**< battery */
	//RSSI = 0x09, /**< rssi */
	//REFRESH = 0x0A, /**< refresh */

	//TEMP = 0x0A, /**< temperature*/
	//BST = 0x0B, /**< boiler status*/
	//HUM = 0x01, /**< humidity */
	//BAR = 0x02, /**< pressure*/
	//OPCL = 0x03, /**< open/closed sensor*/
	//ONOFFSEN = 0x04, /**< on/off sensor*/
	//LUM = 0x05, /**< intensity of light*/
	//EMI = 0x06, /**< emissions*/
	//REZ = 0x07, /**< resonance*/
	//POS = 0x08, /**<position */
	//ONOFSW = 0xA0, /**< on/off switch*/
	//ONON = 0xA1, /**< only on actuator*/
	//TOG = 0xA2, /**< toggle actuator*/
	//RAN = 0xA3, /**< range*/
	//RGB = 0xA4, /**< red green blue actuator*/
	//BT = 0xA5, /**< boiler temperature*/
	//BOT = 0xA6, /**< boiler operation type*/
	//BOM = 0xA7, /**< boiler operation mode*/
	//UNK = 0x00 /**< unknown value*/
//} tvalueTypes;

/** @enum deviceType
 *  @brief Enumerator for types of device according to their values
 */

typedef enum deviceType
{
	UNDEF, /**< undefined*/
	SEN, /**< sensor only*/
	ACT, /**< actuator only*/
	SENACT /**< sensor and actuator*/
} tdeviceType;

/** @enum reqType
 *  @brief Enumerator for types of requests
 */

typedef enum reqType
{
	DATA,  /**< data*/
	REGISTER, /**< register*/
	SWITCH, /**< switch*/
	DELETE, /**< delete*/
	LISTEN, /**< listen*/
  SEARCH, /**< search*/
  GET_PARAMS, /**< get parameters*/
	UNKNOWN /**< unknown*/
}treqType;


/** @struct value
 *  Structure saving value and type
 */
typedef struct value
{
	unsigned short int module_id;
	float measured_value;
  std::string status;
	//unsigned short int intType; /**< integer representation of type*/
	//unsigned short int offset;  /**< offset of value in device*/
	//tvalueTypes  type; /**< type of value*/
	/** @union reqType
	 *  @brief union to save value according to data type
	 */
	//union
	//{
	//	bool bval; /**< boolean type*/
	//	float fval; /**< float type*/
	//	int ival; /**< integer type*/
	//};
} tvalue;

/** @struct xml_string_writer
 *  @brief writes XML structure from memory to std::string
 */

typedef struct xml_string_writer: pugi::xml_writer
{
    std::string result; /**< result*/
    /**Virtual metod to write result to string
     * @param void pointer data to write
     * @param size_t of data to write
    				 */
    virtual void write(const void* data, size_t size)
    {
        result += std::string(static_cast<const char*>(data), size);
    }
} tstringXMLwriter;


/** @struct value
 *  @brief Saving parsed message
 */

typedef struct message
{
	reqType state; /**< request type*/
	long long int adapterINTid; /**< ID of adapter*/
	int fm_version; /**< firmware version of adapter*/
	float cp_version; /**< communication protocol version*/
	time_t timestamp; /**< timestamp of message*/
	int socket; /**< communication socket*/
	unsigned long long int device_type; /**< ID of device*/ // in protocol from senzor it's device_id, in database it's more fitting device_type
	unsigned long long int device_euid;

	std::string DeviceIDstr; /**< Device ID string*/
  std::string DeviceIPstr; /**< Device IP string*/
	
	unsigned short int values_count; /**< count of values received*/
	tvalue* values; /**< pointer to vales array*/
	tdeviceType devType; /**< device type*/
	/** Destructor
	 */
	~message() {
		delete [] values;
		values = NULL;
	}
	/** Constructor
		 */
	message() {
		this->values = NULL;
		this->devType = UNDEF;
		this->DeviceIDstr = "";
		this->adapterINTid = 0;
		this->socket = 0;
		this->cp_version = 0.0;
		this->state = UNKNOWN;
		this->values_count = 0;
		this->fm_version = 0;
		this->timestamp = 0;
		this->device_type = 0; 
		this->device_euid = 0;
	}
} tmessage;

/** @struct testMessage
 *  message type used once for testing inherits from tmessage
 */

typedef struct testMessage : public message
{
	int sensor_port; /**< port of testing sensor*/
} ttestMessage;


#endif /* STRUCTURES_H_ */
