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
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1360890531/DAO.o \
	${OBJECTDIR}/_ext/1360890531/DAOAdapters.o \
	${OBJECTDIR}/_ext/1360890531/DAODevices.o \
	${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o \
	${OBJECTDIR}/_ext/1360890531/DAONotification.o \
	${OBJECTDIR}/_ext/1360890531/DAOPushNotificationService.o \
	${OBJECTDIR}/_ext/1360890531/DAORooms.o \
	${OBJECTDIR}/_ext/1360890531/DAOUsers.o \
	${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o \
	${OBJECTDIR}/_ext/1360890531/DAOlogs.o \
	${OBJECTDIR}/_ext/1360930230/pugixml.o \
	${OBJECTDIR}/_ext/1154895860/Logger.o \
	${OBJECTDIR}/Config.o \
	${OBJECTDIR}/DBConnector.o \
	${OBJECTDIR}/MsgInFactory.o \
	${OBJECTDIR}/ServerException.o \
	${OBJECTDIR}/SessionsTable.o \
	${OBJECTDIR}/SocketClient.o \
	${OBJECTDIR}/SocketServer.o \
	${OBJECTDIR}/TokenChecker.o \
	${OBJECTDIR}/communication.o \
	${OBJECTDIR}/fTokenChecker.o \
	${OBJECTDIR}/gTokenChecker.o \
	${OBJECTDIR}/msgs/AccountAdd.o \
	${OBJECTDIR}/msgs/AccountDel.o \
	${OBJECTDIR}/msgs/AccountGet.o \
	${OBJECTDIR}/msgs/AccountUpdate.o \
	${OBJECTDIR}/msgs/DeviceDelete.o \
	${OBJECTDIR}/msgs/DevicesGet.o \
	${OBJECTDIR}/msgs/DevicesGetAll.o \
	${OBJECTDIR}/msgs/DevicesGetNew.o \
	${OBJECTDIR}/msgs/DevicesUpdate.o \
	${OBJECTDIR}/msgs/GateAdd.o \
	${OBJECTDIR}/msgs/GateDelete.o \
	${OBJECTDIR}/msgs/GateGetInfo.o \
	${OBJECTDIR}/msgs/GateScanMode.o \
	${OBJECTDIR}/msgs/GateUpdate.o \
	${OBJECTDIR}/msgs/GatesGetConnected.o \
	${OBJECTDIR}/msgs/IMsgIn.o \
	${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o \
	${OBJECTDIR}/msgs/IMsgInLoginRequired.o \
	${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o \
	${OBJECTDIR}/msgs/LocationAdd.o \
	${OBJECTDIR}/msgs/LocationDelete.o \
	${OBJECTDIR}/msgs/LocationsGet.o \
	${OBJECTDIR}/msgs/LocationsUpdate.o \
	${OBJECTDIR}/msgs/LogGet.o \
	${OBJECTDIR}/msgs/ModuleSwitchState.o \
	${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o \
	${OBJECTDIR}/msgs/MsgInGamificationRedirect.o \
	${OBJECTDIR}/msgs/MsgInUnknown.o \
	${OBJECTDIR}/msgs/NotificationEraseGCMID.o \
	${OBJECTDIR}/msgs/NotificationReaded.o \
	${OBJECTDIR}/msgs/NotificationSetGCMID.o \
	${OBJECTDIR}/msgs/NotificationsGet.o \
	${OBJECTDIR}/msgs/UserGetInfo.o \
	${OBJECTDIR}/msgs/UserLogIn.o \
	${OBJECTDIR}/msgs/UserLogout.o \
	${OBJECTDIR}/msgs/UserRegister.o \
	${OBJECTDIR}/save_custom_writer.o \
	${OBJECTDIR}/sslServlet.o \
	${OBJECTDIR}/ui_server.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++0x
CXXFLAGS=-std=c++0x

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib -L/usr/local/lib64 -L/opt/centos/devtoolset-1.1/root/usr/lib/gcc/x86_64-redhat-linux/4.7.2 -L/usr/pgsql-9.2/lib -L../lib -L/usr/lib64 -Wl,-rpath,/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,../lib -Wl,-rpath,/usr/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -lpq -ljansson

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ./ui_server

./ui_server: ${OBJECTFILES}
	${MKDIR} -p .
	g++ -o ./ui_server ${OBJECTFILES} ${LDLIBSOPTIONS} -lpq -lsoci_core -lsoci_empty -lsoci_postgresql -lcurl -lssl -lcrypto -ljansson -lstdc++

${OBJECTDIR}/_ext/1360890531/DAO.o: ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp

${OBJECTDIR}/_ext/1360890531/DAOAdapters.o: ../DAO/DAOAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters.o ../DAO/DAOAdapters.cpp

${OBJECTDIR}/_ext/1360890531/DAODevices.o: ../DAO/DAODevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices.o ../DAO/DAODevices.cpp

${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o: ../DAO/DAOMobileDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o ../DAO/DAOMobileDevices.cpp

${OBJECTDIR}/_ext/1360890531/DAONotification.o: ../DAO/DAONotification.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAONotification.o ../DAO/DAONotification.cpp

${OBJECTDIR}/_ext/1360890531/DAOPushNotificationService.o: ../DAO/DAOPushNotificationService.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOPushNotificationService.o ../DAO/DAOPushNotificationService.cpp

${OBJECTDIR}/_ext/1360890531/DAORooms.o: ../DAO/DAORooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms.o ../DAO/DAORooms.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsers.o: ../DAO/DAOUsers.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers.o ../DAO/DAOUsers.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o: ../DAO/DAOUsersAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o ../DAO/DAOUsersAdapters.cpp

${OBJECTDIR}/_ext/1360890531/DAOlogs.o: ../DAO/DAOlogs.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOlogs.o ../DAO/DAOlogs.cpp

${OBJECTDIR}/_ext/1360930230/pugixml.o: ../lib/pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360930230
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360930230/pugixml.o ../lib/pugixml.cpp

${OBJECTDIR}/_ext/1154895860/Logger.o: ../ui_logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1154895860
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger.o ../ui_logger/Logger.cpp

${OBJECTDIR}/Config.o: Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config.o Config.cpp

${OBJECTDIR}/DBConnector.o: DBConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector.o DBConnector.cpp

${OBJECTDIR}/MsgInFactory.o: MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp

${OBJECTDIR}/ServerException.o: ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException.o ServerException.cpp

${OBJECTDIR}/SessionsTable.o: SessionsTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SessionsTable.o SessionsTable.cpp

${OBJECTDIR}/SocketClient.o: SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient.o SocketClient.cpp

${OBJECTDIR}/SocketServer.o: SocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer.o SocketServer.cpp

${OBJECTDIR}/TokenChecker.o: TokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TokenChecker.o TokenChecker.cpp

${OBJECTDIR}/communication.o: communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication.o communication.cpp

${OBJECTDIR}/fTokenChecker.o: fTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fTokenChecker.o fTokenChecker.cpp

${OBJECTDIR}/gTokenChecker.o: gTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker.o gTokenChecker.cpp

${OBJECTDIR}/msgs/AccountAdd.o: msgs/AccountAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountAdd.o msgs/AccountAdd.cpp

${OBJECTDIR}/msgs/AccountDel.o: msgs/AccountDel.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountDel.o msgs/AccountDel.cpp

${OBJECTDIR}/msgs/AccountGet.o: msgs/AccountGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountGet.o msgs/AccountGet.cpp

${OBJECTDIR}/msgs/AccountUpdate.o: msgs/AccountUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountUpdate.o msgs/AccountUpdate.cpp

${OBJECTDIR}/msgs/DeviceDelete.o: msgs/DeviceDelete.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DeviceDelete.o msgs/DeviceDelete.cpp

${OBJECTDIR}/msgs/DevicesGet.o: msgs/DevicesGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesGet.o msgs/DevicesGet.cpp

${OBJECTDIR}/msgs/DevicesGetAll.o: msgs/DevicesGetAll.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesGetAll.o msgs/DevicesGetAll.cpp

${OBJECTDIR}/msgs/DevicesGetNew.o: msgs/DevicesGetNew.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesGetNew.o msgs/DevicesGetNew.cpp

${OBJECTDIR}/msgs/DevicesUpdate.o: msgs/DevicesUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesUpdate.o msgs/DevicesUpdate.cpp

${OBJECTDIR}/msgs/GateAdd.o: msgs/GateAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateAdd.o msgs/GateAdd.cpp

${OBJECTDIR}/msgs/GateDelete.o: msgs/GateDelete.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateDelete.o msgs/GateDelete.cpp

${OBJECTDIR}/msgs/GateGetInfo.o: msgs/GateGetInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateGetInfo.o msgs/GateGetInfo.cpp

${OBJECTDIR}/msgs/GateScanMode.o: msgs/GateScanMode.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateScanMode.o msgs/GateScanMode.cpp

${OBJECTDIR}/msgs/GateUpdate.o: msgs/GateUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateUpdate.o msgs/GateUpdate.cpp

${OBJECTDIR}/msgs/GatesGetConnected.o: msgs/GatesGetConnected.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GatesGetConnected.o msgs/GatesGetConnected.cpp

${OBJECTDIR}/msgs/IMsgIn.o: msgs/IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgIn.o msgs/IMsgIn.cpp

${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o: msgs/IMsgInLoginAndAdapterAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o msgs/IMsgInLoginAndAdapterAccessRequired.cpp

${OBJECTDIR}/msgs/IMsgInLoginRequired.o: msgs/IMsgInLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginRequired.o msgs/IMsgInLoginRequired.cpp

${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o: msgs/IMsgInLoginUnwanted.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o msgs/IMsgInLoginUnwanted.cpp

${OBJECTDIR}/msgs/LocationAdd.o: msgs/LocationAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationAdd.o msgs/LocationAdd.cpp

${OBJECTDIR}/msgs/LocationDelete.o: msgs/LocationDelete.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationDelete.o msgs/LocationDelete.cpp

${OBJECTDIR}/msgs/LocationsGet.o: msgs/LocationsGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationsGet.o msgs/LocationsGet.cpp

${OBJECTDIR}/msgs/LocationsUpdate.o: msgs/LocationsUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationsUpdate.o msgs/LocationsUpdate.cpp

${OBJECTDIR}/msgs/LogGet.o: msgs/LogGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LogGet.o msgs/LogGet.cpp

${OBJECTDIR}/msgs/ModuleSwitchState.o: msgs/ModuleSwitchState.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/ModuleSwitchState.o msgs/ModuleSwitchState.cpp

${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o: msgs/MsgInAlgorithmsRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o msgs/MsgInAlgorithmsRedirect.cpp

${OBJECTDIR}/msgs/MsgInGamificationRedirect.o: msgs/MsgInGamificationRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o msgs/MsgInGamificationRedirect.cpp

${OBJECTDIR}/msgs/MsgInUnknown.o: msgs/MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUnknown.o msgs/MsgInUnknown.cpp

${OBJECTDIR}/msgs/NotificationEraseGCMID.o: msgs/NotificationEraseGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationEraseGCMID.o msgs/NotificationEraseGCMID.cpp

${OBJECTDIR}/msgs/NotificationReaded.o: msgs/NotificationReaded.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationReaded.o msgs/NotificationReaded.cpp

${OBJECTDIR}/msgs/NotificationSetGCMID.o: msgs/NotificationSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationSetGCMID.o msgs/NotificationSetGCMID.cpp

${OBJECTDIR}/msgs/NotificationsGet.o: msgs/NotificationsGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationsGet.o msgs/NotificationsGet.cpp

${OBJECTDIR}/msgs/UserGetInfo.o: msgs/UserGetInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetInfo.o msgs/UserGetInfo.cpp

${OBJECTDIR}/msgs/UserLogIn.o: msgs/UserLogIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogIn.o msgs/UserLogIn.cpp

${OBJECTDIR}/msgs/UserLogout.o: msgs/UserLogout.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogout.o msgs/UserLogout.cpp

${OBJECTDIR}/msgs/UserRegister.o: msgs/UserRegister.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserRegister.o msgs/UserRegister.cpp

${OBJECTDIR}/save_custom_writer.o: save_custom_writer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer.o save_custom_writer.cpp

${OBJECTDIR}/sslServlet.o: sslServlet.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet.o sslServlet.cpp

${OBJECTDIR}/ui_server.o: ui_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server.o ui_server.cpp

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
