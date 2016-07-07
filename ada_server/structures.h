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
#include <openssl/crypto.h>
#include <vector>

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
	PING, /*< empty get parameters to adapter>*/
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

typedef struct params
{
	int id;
	unsigned long long int euid;
	std::string value;
	std::vector<unsigned long long> *deviceList;
	double measured_value;
	unsigned int module_id;
	bool valid_data;
    params()
    {
        id = 0;
        euid = 0;
        value = "";
        module_id = 0;
        measured_value = 0;
		deviceList = nullptr;
        valid_data = false;
    };
	~params()
	{
		if (this->deviceList!= nullptr)
			delete deviceList;
	}
} tparams;

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

typedef struct messageV1_0
{
	reqType state; /**< request type*/
	long long int adapterINTid; /**< ID of adapter*/
	int fm_version; /**< firmware version of adapter*/
	float cp_version; /**< communication protocol version*/
	time_t timestamp; /**< timestamp of message*/
	int socket; /**< communication socket*/
	unsigned int module_id; /**< module ID*/
	unsigned long long int device_type; /**< ID of device*/ // in protocol from senzor it's device_id, in database it's more fitting device_type
	unsigned long long int device_euid;
	unsigned int refresh;
	std::string DeviceIDstr; /**< Device ID string*/
  	std::string DeviceIPstr; /**< Device IP string*/
	bool registerResult;
	unsigned short int values_count; /**< count of values received*/
	tvalue* values; /**< pointer to vales array*/
	tdeviceType devType; /**< device type*/
	/** Destructor
	 */
	~messageV1_0() {
		delete [] values;
		values = NULL;
	}
	/** Constructor
		 */
	messageV1_0() {
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
		this->module_id = 0;
		this->device_type = 0; 
		this->device_euid = 0;
		this->refresh = 5;
		this->registerResult = false;
	}
} tmessageV1_0;

typedef struct messageV1_1 : public messageV1_0
{
	size_t processedParams;
	messageV1_1() : tmessageV1_0()
	{
		processedParams = 0;
        params = new std::vector<tparams*>();
	}
    ~messageV1_1()
    {
        for (size_t i=0;i<this->params->size();i++)
        {
            delete params->at(i);
        }
        delete params;
    }
    std::vector<tparams*> *params;
} tmessageV1_1;

/** @struct testMessage
 *  message type used once for testing inherits from tmessageV1_0
 */

typedef struct testMessage : public messageV1_0
{
	int sensor_port; /**< port of testing sensor*/
} ttestMessage;


typedef struct adapter {
	SSL* connection;
	float protocol_version;
	adapter(SSL *conn,float cp)
	{
		this->connection=conn;
		protocol_version = cp;
	}
} tadapter;


#endif /* STRUCTURES_H_ */
