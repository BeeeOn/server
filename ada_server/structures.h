/*
 * structures.h
 *
 *  Created on: Oct 26, 2014
 *      Author: tuso
 */

#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include <string>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

/** @enum valueTypes
 *  @brief Enumerator sluziaci na zaznamenanie typu hodnoty posielanej zo zariadenia
 */

typedef enum valueTypes
{
	TEMP = 0x0A,
	HUM = 0x01,
	BAR = 0x02,
	ONOFFSEN = 0x03,
	LUM = 0x05,
	EMI = 0x06,
	REZ = 0x07,
	POS = 0x08,
	ONOFSW = 0xA0,
	ONON = 0xA1,
	TOG = 0xA2,
	RAN = 0xA3,
	RGB = 0xA4,
	UNK = 0x00
} tvalueTypes;


typedef enum deviceType
{
	UNDEF,
	SEN,
	ACT,
	SENACT
} tdeviceType;

typedef enum reqType
{
	DATA,
	REGISTER,
	SWITCH,
	DELETE,
	LISTEN,
	UNKNOWN
}treqType;


/** @struct value
 *  @brief Struktura uchovavajuca hodnoty prijate od adapteru a zariadenia.
 *  @var value::type
 *  Clen 'type' obsahuje typ hodnoty.
 *  @var value::bval
 *  Clen 'bval' obsahuje booleanovsku hodnotu
 *  @var value::fval
 *  Clen 'fval' obsahuje hodnotu s plavajucou desatinnou ciarkou.
 */
typedef struct value
{
	unsigned short int intType;
	unsigned short int offset;
	tvalueTypes  type;
	union
	{
		bool bval;
		float fval;
		int ival;
	};
} tvalue;

typedef struct xml_string_writer: pugi::xml_writer
{
    std::string result;

    virtual void write(const void* data, size_t size)
    {
        result += std::string(static_cast<const char*>(data), size);
    }
} tstringXMLwriter;


/** @struct value
 *  @brief Struktura uchovavajuca spracovanu spravu od adapteru a zariadenia.
 *  @var message::adapter_id
 *  Clen 'adapter_id' obsahuje znakove ID adaptera.
 *  @var message::adapterINTid
 *  Clen 'adapterINTid' obsahuje integerovu hodnotu adaptera
 *  @var message::adapter_ip
 *  Clen 'adapter_ip' obsahuje ip adresu adaptera
 * @var message::fm_version
 *  Clen 'fm_version' obsahuje verziu firmwaru adaptera
 * @var message::byte_fm_version
 *  Clen 'byte_fm_version' obsahuje bytovu verziu firmwaru adaptera pre rychlejsie tvorenie odpovede
 * @var message::cp_version
 *  Clen 'cp_version' obsahuje verziu komunikacneho protokolu adaptera
 * @var message::byte_cp_version
 *  Clen 'byte_cp_version' obsahuje bytovu verziu komunikacneho protokolu adaptera pre rychlejsie tvorenie odpovede
 * @var message::timestamp
 *  Clen 'timestamp' obsahuje casove razitko
 * @var message::sensor_id
 *  Clen 'sensor_id' obsahuje ip adresu zariadenia
 * @var message::byte_sensor_id
 *  Clen 'byte_sensor_id' obsahuje bytovu ip adresu zariadenia
 * @var message::sensor_port
 *  Clen 'sensor_port' obsahuje port zariadenia
 * @var message::byte_sensor_port
 *  Clen 'byte_sensor_port' obsahuje bytovy port zariadenia
 * @var message::deviceProcolVersion
 *  Clen 'deviceProcolVersion' obsahuje verziu protokolu zariadenia
 * @var message::battery
 *  Clen 'battery' obsahuje hodnotu baterie zariadenia
 * @var message::signal_strength
 *  Clen 'signal_strength' obsahuje silu signalu zariadenia
 * @var message::values_count
 *  Clen 'values_count' obsahuje pocet prenasanych dvojic typ hodnota
 * @var message::values
 *  Clen 'values' obsahuje ukazatel na pole dvojic typ hodnota
 */

typedef struct message
{
	reqType state;
	long long int adapterINTid;
	int fm_version;
	float cp_version;
	time_t timestamp;
	int socket;
	unsigned long long int sensor_id;
	std::string DeviceIDstr;
	unsigned short int battery;
	unsigned short int signal_strength;
	unsigned short int values_count;
	tvalue* values;
	tdeviceType devType;
	~message();
	message();
} tmessage;

typedef struct testMessage : public message
{
	int sensor_port;
} ttestMessage;


#endif /* STRUCTURES_H_ */
