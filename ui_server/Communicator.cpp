#include "stdafx.h"

#include "Communicator.h"
#include "MsgInFactory.h"
#include "IMsgOut.h"
#include "DBConnector.h"

using namespace std;
Communicator::Communicator(void)
{
	_id = 123;
}


Communicator::~Communicator(void)
{
}

void Communicator::resolveMsg(char* msg)
{
	msg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><communication id=\"0\" state=\"signin\"><user email=\"user1@gmail.com\"/></communication>";	
	//<?xml version="1.0" encoding="UTF-8"?><communication version="1.0" id="425484" state="init"><adapter id="01233145645792" /></communication>
	//msg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><communication version=\"1.0\" id=\"10\" state=\"init\"><adapter id=\"10\" /></communication>";
	//msg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><communication version=\"1.0\" id=\"10\" state=\"update\">"
	//"<device id=\"00:00:00:99:99:99\"/>  <device id=\"08:00:2b:01:02:03\"/>  </communication>";
	
	MsgInFactory msgFactory = MsgInFactory::getInstance();
	
	//DBConnector::getInstance().insertNewAdapter("100", "testad", "1", "anotheruser@yahoo.com");
	/*std::string state = doc.child("communication").attribute("state").value();

	std::cout << "new message IN type:"<< state<< std::endl;*/

	

	std::cout << "creating msgIn" << std::endl;
	IMsgIn* msgIn = msgFactory.createMsg(msg);

	std::cout << "creating msgOut" << std::endl;
	IMsgOut* msgOut = createResponse(msgIn);
	
	sendMsgOut(msgOut);

	delete msgIn;
	
	//free msg
}
IMsgOut* Communicator::createResponse(IMsgIn* msgIn)
{
	IMsgOut* msgOut = msgIn->createResponseMsgOut();
	return msgOut;
}

void Communicator::sendMsgOut(IMsgOut* msgOut)
{
	std::stringstream ss;
	std::cout<<std::endl<<"OUT:"<<std::endl;
	ss << "<communication id=\"" << _id <<  "\" state=\"" << msgOut->getState() <<"\">" << msgOut->makeResponse() << "</communication>" << std::endl;
	
	pugi::xml_document docOut;
	docOut.load(ss.str().c_str());
	docOut.save(std::cout, "\t", pugi::format_indent);
}
