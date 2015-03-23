/* 
 * File:   ui_server.h
 * Author: pavel
 *
 * Created on 14. listopad 2014, 16:43
 */

#ifndef UI_SERVER_H
#define	UI_SERVER_H

#include "communication.h"
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

#include <string>
#include <iostream>
#include "../ui_logger/Logger.h"
#include "DBConnector.h"
#include "gTokenChecker.h"
#include "ComTable.h"
#include "pugixml.hpp"
#include "MsgRightsChecker.h"
#include "ServerException.h"
#include "SocketClient.h"
#include "Config.h"
#include <signal.h>
#include <atomic>

#include "testHelper.h"
#include "sslServlet.h"



#endif	/* UI_SERVER_H */

