#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/511d8aa3/DAO.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOContainer.o \
	${OBJECTDIR}/_ext/511d8aa3/DAODevices.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOGateways.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOLocations.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOMobileDevices.o \
	${OBJECTDIR}/_ext/511d8aa3/DAONotification.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOPushNotificationService.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOUsers.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOUsersGateways.o \
	${OBJECTDIR}/_ext/511d8aa3/DAOlogs.o \
	${OBJECTDIR}/_ext/3dfcc76d/client.o \
	${OBJECTDIR}/_ext/3dfcc76d/server.o \
	${OBJECTDIR}/_ext/3dfcc76d/session.o \
	${OBJECTDIR}/_ext/511e25b6/pugixml.o \
	${OBJECTDIR}/_ext/bb29b00c/Logger.o \
	${OBJECTDIR}/src/AdaServerCommunicator.o \
	${OBJECTDIR}/src/Config.o \
	${OBJECTDIR}/src/RequestHandlerFactory.o \
	${OBJECTDIR}/src/SecuredAsioServlet.o \
	${OBJECTDIR}/src/ServerException.o \
	${OBJECTDIR}/src/SessionsTable.o \
	${OBJECTDIR}/src/SocketClient.o \
	${OBJECTDIR}/src/SocketServer.o \
	${OBJECTDIR}/src/TokenChecker.o \
	${OBJECTDIR}/src/request_handlers/AccountGetProfileRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/AccountsLoginRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/AccountsLogoutRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/AccountsRegisterRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/DevicesGetAllRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/DevicesGetLogRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/DevicesGetNewRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/DevicesGetRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/DevicesSetStateRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/DevicesUnregisterRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/DevicesUpdateRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GatesGetAllRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GatesGetRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GatesRegisterRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GatesStartListenRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GatesUnregisterRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GatesUpdateRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GateusersGetAllRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GateusersInviteRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GateusersRemoveRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/GateusersUpdateRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/IRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/IRequestHandlerWithLoginAndGateAccessRequired.o \
	${OBJECTDIR}/src/request_handlers/IRequestHandlerWithLoginRequired.o \
	${OBJECTDIR}/src/request_handlers/IRequestHandlerWithoutUac.o \
	${OBJECTDIR}/src/request_handlers/LocationsAddRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/LocationsDeleteRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/LocationsGetAllRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/LocationsUpdateRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/NotificationRegisterServiceRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/NotificationsGetRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/NotificationsSetReadRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/NotificationsUnregisterServiceRequestHandler.o \
	${OBJECTDIR}/src/request_handlers/UnknownRequestHandler.o \
	${OBJECTDIR}/src/save_custom_writer.o \
	${OBJECTDIR}/src/sslServlet.o \
	${OBJECTDIR}/src/ui_server.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../soci-3.2.2/build/lib -L/usr/local/include/soci -L../soci-3.2.2/soci-3.2.2/backends/postgresql -L../soci-3.2.2/soci-3.2.2/core -L/usr/include/postgresql -L/usr/include/postgresql/libpq -L. -L/usr/include -L/usr/local/include -L/usr/local/lib -L/usr/lib -Wl,-rpath,../soci-3.2.2/build/lib -Wl,-rpath,/usr/local/lib -lpq

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ./ui_server

./ui_server: ${OBJECTFILES}
	${MKDIR} -p .
	${LINK.cc} -o ./ui_server ${OBJECTFILES} ${LDLIBSOPTIONS} -lpq -lsoci_core -lsoci_empty -lsoci_postgresql -lssl -lcrypto -ljansson

${OBJECTDIR}/_ext/511d8aa3/DAO.o: ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAO.o ../DAO/DAO.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOContainer.o: ../DAO/DAOContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOContainer.o ../DAO/DAOContainer.cpp

${OBJECTDIR}/_ext/511d8aa3/DAODevices.o: ../DAO/DAODevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAODevices.o ../DAO/DAODevices.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOGateways.o: ../DAO/DAOGateways.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOGateways.o ../DAO/DAOGateways.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOLocations.o: ../DAO/DAOLocations.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOLocations.o ../DAO/DAOLocations.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOMobileDevices.o: ../DAO/DAOMobileDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOMobileDevices.o ../DAO/DAOMobileDevices.cpp

${OBJECTDIR}/_ext/511d8aa3/DAONotification.o: ../DAO/DAONotification.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAONotification.o ../DAO/DAONotification.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOPushNotificationService.o: ../DAO/DAOPushNotificationService.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOPushNotificationService.o ../DAO/DAOPushNotificationService.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOUsers.o: ../DAO/DAOUsers.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOUsers.o ../DAO/DAOUsers.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOUsersGateways.o: ../DAO/DAOUsersGateways.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOUsersGateways.o ../DAO/DAOUsersGateways.cpp

${OBJECTDIR}/_ext/511d8aa3/DAOlogs.o: ../DAO/DAOlogs.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511d8aa3
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511d8aa3/DAOlogs.o ../DAO/DAOlogs.cpp

${OBJECTDIR}/_ext/3dfcc76d/client.o: ../asio_layer/secure/client.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/3dfcc76d
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/3dfcc76d/client.o ../asio_layer/secure/client.cpp

${OBJECTDIR}/_ext/3dfcc76d/server.o: ../asio_layer/secure/server.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/3dfcc76d
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/3dfcc76d/server.o ../asio_layer/secure/server.cpp

${OBJECTDIR}/_ext/3dfcc76d/session.o: ../asio_layer/secure/session.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/3dfcc76d
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/3dfcc76d/session.o ../asio_layer/secure/session.cpp

${OBJECTDIR}/_ext/511e25b6/pugixml.o: ../lib/pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511e25b6
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e25b6/pugixml.o ../lib/pugixml.cpp

${OBJECTDIR}/_ext/bb29b00c/Logger.o: ../ui_logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/bb29b00c
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/bb29b00c/Logger.o ../ui_logger/Logger.cpp

${OBJECTDIR}/src/AdaServerCommunicator.o: src/AdaServerCommunicator.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AdaServerCommunicator.o src/AdaServerCommunicator.cpp

${OBJECTDIR}/src/Config.o: src/Config.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Config.o src/Config.cpp

${OBJECTDIR}/src/RequestHandlerFactory.o: src/RequestHandlerFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RequestHandlerFactory.o src/RequestHandlerFactory.cpp

${OBJECTDIR}/src/SecuredAsioServlet.o: src/SecuredAsioServlet.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SecuredAsioServlet.o src/SecuredAsioServlet.cpp

${OBJECTDIR}/src/ServerException.o: src/ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ServerException.o src/ServerException.cpp

${OBJECTDIR}/src/SessionsTable.o: src/SessionsTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SessionsTable.o src/SessionsTable.cpp

${OBJECTDIR}/src/SocketClient.o: src/SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SocketClient.o src/SocketClient.cpp

${OBJECTDIR}/src/SocketServer.o: src/SocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SocketServer.o src/SocketServer.cpp

${OBJECTDIR}/src/TokenChecker.o: src/TokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TokenChecker.o src/TokenChecker.cpp

${OBJECTDIR}/src/request_handlers/AccountGetProfileRequestHandler.o: src/request_handlers/AccountGetProfileRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/AccountGetProfileRequestHandler.o src/request_handlers/AccountGetProfileRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/AccountsLoginRequestHandler.o: src/request_handlers/AccountsLoginRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/AccountsLoginRequestHandler.o src/request_handlers/AccountsLoginRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/AccountsLogoutRequestHandler.o: src/request_handlers/AccountsLogoutRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/AccountsLogoutRequestHandler.o src/request_handlers/AccountsLogoutRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/AccountsRegisterRequestHandler.o: src/request_handlers/AccountsRegisterRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/AccountsRegisterRequestHandler.o src/request_handlers/AccountsRegisterRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/DevicesGetAllRequestHandler.o: src/request_handlers/DevicesGetAllRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/DevicesGetAllRequestHandler.o src/request_handlers/DevicesGetAllRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/DevicesGetLogRequestHandler.o: src/request_handlers/DevicesGetLogRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/DevicesGetLogRequestHandler.o src/request_handlers/DevicesGetLogRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/DevicesGetNewRequestHandler.o: src/request_handlers/DevicesGetNewRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/DevicesGetNewRequestHandler.o src/request_handlers/DevicesGetNewRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/DevicesGetRequestHandler.o: src/request_handlers/DevicesGetRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/DevicesGetRequestHandler.o src/request_handlers/DevicesGetRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/DevicesSetStateRequestHandler.o: src/request_handlers/DevicesSetStateRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/DevicesSetStateRequestHandler.o src/request_handlers/DevicesSetStateRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/DevicesUnregisterRequestHandler.o: src/request_handlers/DevicesUnregisterRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/DevicesUnregisterRequestHandler.o src/request_handlers/DevicesUnregisterRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/DevicesUpdateRequestHandler.o: src/request_handlers/DevicesUpdateRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/DevicesUpdateRequestHandler.o src/request_handlers/DevicesUpdateRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GatesGetAllRequestHandler.o: src/request_handlers/GatesGetAllRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GatesGetAllRequestHandler.o src/request_handlers/GatesGetAllRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GatesGetRequestHandler.o: src/request_handlers/GatesGetRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GatesGetRequestHandler.o src/request_handlers/GatesGetRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GatesRegisterRequestHandler.o: src/request_handlers/GatesRegisterRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GatesRegisterRequestHandler.o src/request_handlers/GatesRegisterRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GatesStartListenRequestHandler.o: src/request_handlers/GatesStartListenRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GatesStartListenRequestHandler.o src/request_handlers/GatesStartListenRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GatesUnregisterRequestHandler.o: src/request_handlers/GatesUnregisterRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GatesUnregisterRequestHandler.o src/request_handlers/GatesUnregisterRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GatesUpdateRequestHandler.o: src/request_handlers/GatesUpdateRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GatesUpdateRequestHandler.o src/request_handlers/GatesUpdateRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GateusersGetAllRequestHandler.o: src/request_handlers/GateusersGetAllRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GateusersGetAllRequestHandler.o src/request_handlers/GateusersGetAllRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GateusersInviteRequestHandler.o: src/request_handlers/GateusersInviteRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GateusersInviteRequestHandler.o src/request_handlers/GateusersInviteRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GateusersRemoveRequestHandler.o: src/request_handlers/GateusersRemoveRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GateusersRemoveRequestHandler.o src/request_handlers/GateusersRemoveRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/GateusersUpdateRequestHandler.o: src/request_handlers/GateusersUpdateRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/GateusersUpdateRequestHandler.o src/request_handlers/GateusersUpdateRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/IRequestHandler.o: src/request_handlers/IRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/IRequestHandler.o src/request_handlers/IRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/IRequestHandlerWithLoginAndGateAccessRequired.o: src/request_handlers/IRequestHandlerWithLoginAndGateAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/IRequestHandlerWithLoginAndGateAccessRequired.o src/request_handlers/IRequestHandlerWithLoginAndGateAccessRequired.cpp

${OBJECTDIR}/src/request_handlers/IRequestHandlerWithLoginRequired.o: src/request_handlers/IRequestHandlerWithLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/IRequestHandlerWithLoginRequired.o src/request_handlers/IRequestHandlerWithLoginRequired.cpp

${OBJECTDIR}/src/request_handlers/IRequestHandlerWithoutUac.o: src/request_handlers/IRequestHandlerWithoutUac.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/IRequestHandlerWithoutUac.o src/request_handlers/IRequestHandlerWithoutUac.cpp

${OBJECTDIR}/src/request_handlers/LocationsAddRequestHandler.o: src/request_handlers/LocationsAddRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/LocationsAddRequestHandler.o src/request_handlers/LocationsAddRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/LocationsDeleteRequestHandler.o: src/request_handlers/LocationsDeleteRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/LocationsDeleteRequestHandler.o src/request_handlers/LocationsDeleteRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/LocationsGetAllRequestHandler.o: src/request_handlers/LocationsGetAllRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/LocationsGetAllRequestHandler.o src/request_handlers/LocationsGetAllRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/LocationsUpdateRequestHandler.o: src/request_handlers/LocationsUpdateRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/LocationsUpdateRequestHandler.o src/request_handlers/LocationsUpdateRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/NotificationRegisterServiceRequestHandler.o: src/request_handlers/NotificationRegisterServiceRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/NotificationRegisterServiceRequestHandler.o src/request_handlers/NotificationRegisterServiceRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/NotificationsGetRequestHandler.o: src/request_handlers/NotificationsGetRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/NotificationsGetRequestHandler.o src/request_handlers/NotificationsGetRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/NotificationsSetReadRequestHandler.o: src/request_handlers/NotificationsSetReadRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/NotificationsSetReadRequestHandler.o src/request_handlers/NotificationsSetReadRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/NotificationsUnregisterServiceRequestHandler.o: src/request_handlers/NotificationsUnregisterServiceRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/NotificationsUnregisterServiceRequestHandler.o src/request_handlers/NotificationsUnregisterServiceRequestHandler.cpp

${OBJECTDIR}/src/request_handlers/UnknownRequestHandler.o: src/request_handlers/UnknownRequestHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/request_handlers
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/request_handlers/UnknownRequestHandler.o src/request_handlers/UnknownRequestHandler.cpp

${OBJECTDIR}/src/save_custom_writer.o: src/save_custom_writer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/save_custom_writer.o src/save_custom_writer.cpp

${OBJECTDIR}/src/sslServlet.o: src/sslServlet.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sslServlet.o src/sslServlet.cpp

${OBJECTDIR}/src/ui_server.o: src/ui_server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ui_server.o src/ui_server.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ./ui_server

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
