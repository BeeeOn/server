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
	${OBJECTDIR}/_ext/1360890531/DAORooms.o \
	${OBJECTDIR}/_ext/1360890531/DAOUsers.o \
	${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o \
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
	${OBJECTDIR}/msgs/UserGetID.o \
	${OBJECTDIR}/msgs/UserGetInfo.o \
	${OBJECTDIR}/msgs/UserLogIn.o \
	${OBJECTDIR}/msgs/UserLogout.o \
	${OBJECTDIR}/msgs/UserRegister.o \
	${OBJECTDIR}/save_custom_writer.o \
	${OBJECTDIR}/sslServlet.o \
	${OBJECTDIR}/ui_server.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f21 \
	${TESTDIR}/TestFiles/f15 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f20 \
	${TESTDIR}/TestFiles/f8 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f31 \
	${TESTDIR}/TestFiles/f16 \
	${TESTDIR}/TestFiles/f9 \
	${TESTDIR}/TestFiles/f26 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f19 \
	${TESTDIR}/TestFiles/f29 \
	${TESTDIR}/TestFiles/f28 \
	${TESTDIR}/TestFiles/f18 \
	${TESTDIR}/TestFiles/f17 \
	${TESTDIR}/TestFiles/f24 \
	${TESTDIR}/TestFiles/f23 \
	${TESTDIR}/TestFiles/f7 \
	${TESTDIR}/TestFiles/f14 \
	${TESTDIR}/TestFiles/f11 \
	${TESTDIR}/TestFiles/f27 \
	${TESTDIR}/TestFiles/f22 \
	${TESTDIR}/TestFiles/f13 \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f10 \
	${TESTDIR}/TestFiles/f12 \
	${TESTDIR}/TestFiles/f30 \
	${TESTDIR}/TestFiles/f25 \
	${TESTDIR}/TestFiles/f6

# C Compiler Flags
CFLAGS=`cppunit-config --cflags` 

# CC Compiler Flags
CCFLAGS=-O0 -std=c++0x -D_GLIBCXX_USE_NANOSLEEP `cppunit-config --cflags` 
CXXFLAGS=-O0 -std=c++0x -D_GLIBCXX_USE_NANOSLEEP `cppunit-config --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib -L/usr/local/lib64 -L/opt/centos/devtoolset-1.1/root/usr/lib/gcc/x86_64-redhat-linux/4.7.2 -L/usr/pgsql-9.2/lib -L../lib -L/usr/lib64 -Wl,-rpath,/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,../lib -Wl,-rpath,/usr/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -lpq -ljansson `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs`  

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

${OBJECTDIR}/msgs/UserGetID.o: msgs/UserGetID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetID.o msgs/UserGetID.cpp

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

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f21: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f21 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f15: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f15 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f20: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f20 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f8: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f8 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f4: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f31: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f31 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f16: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f16 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f9: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f9 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f26: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f26 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f5: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f19: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f19 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f29: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f29 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f28: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f28 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f18: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f18 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f17: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f17 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f24: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f24 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f23: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f23 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f7: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f14: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f14 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f11: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f11 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f27: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f27 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f22: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f22 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f13: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f13 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f1: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f2: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f10: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f10 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f12: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f12 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f30: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f30 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f25: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f25 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f6: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   


${OBJECTDIR}/_ext/1360890531/DAO_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAO.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO_nomain.o ../DAO/DAO.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAO.o ${OBJECTDIR}/_ext/1360890531/DAO_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAOAdapters_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAOAdapters.o ../DAO/DAOAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAOAdapters.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters_nomain.o ../DAO/DAOAdapters.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAOAdapters.o ${OBJECTDIR}/_ext/1360890531/DAOAdapters_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAODevices_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAODevices.o ../DAO/DAODevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAODevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices_nomain.o ../DAO/DAODevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAODevices.o ${OBJECTDIR}/_ext/1360890531/DAODevices_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAOMobileDevices_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o ../DAO/DAOMobileDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices_nomain.o ../DAO/DAOMobileDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAONotification_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAONotification.o ../DAO/DAONotification.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAONotification.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAONotification_nomain.o ../DAO/DAONotification.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAONotification.o ${OBJECTDIR}/_ext/1360890531/DAONotification_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAORooms_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAORooms.o ../DAO/DAORooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAORooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms_nomain.o ../DAO/DAORooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAORooms.o ${OBJECTDIR}/_ext/1360890531/DAORooms_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAOUsers_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAOUsers.o ../DAO/DAOUsers.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAOUsers.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers_nomain.o ../DAO/DAOUsers.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAOUsers.o ${OBJECTDIR}/_ext/1360890531/DAOUsers_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o ../DAO/DAOUsersAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters_nomain.o ../DAO/DAOUsersAdapters.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360930230/pugixml_nomain.o: ${OBJECTDIR}/_ext/1360930230/pugixml.o ../lib/pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360930230
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360930230/pugixml.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360930230/pugixml_nomain.o ../lib/pugixml.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360930230/pugixml.o ${OBJECTDIR}/_ext/1360930230/pugixml_nomain.o;\
	fi

${OBJECTDIR}/_ext/1154895860/Logger_nomain.o: ${OBJECTDIR}/_ext/1154895860/Logger.o ../ui_logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1154895860
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1154895860/Logger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger_nomain.o ../ui_logger/Logger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1154895860/Logger.o ${OBJECTDIR}/_ext/1154895860/Logger_nomain.o;\
	fi

${OBJECTDIR}/Config_nomain.o: ${OBJECTDIR}/Config.o Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Config.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config_nomain.o Config.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Config.o ${OBJECTDIR}/Config_nomain.o;\
	fi

${OBJECTDIR}/DBConnector_nomain.o: ${OBJECTDIR}/DBConnector.o DBConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/DBConnector.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector_nomain.o DBConnector.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/DBConnector.o ${OBJECTDIR}/DBConnector_nomain.o;\
	fi

${OBJECTDIR}/MsgInFactory_nomain.o: ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInFactory.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory_nomain.o MsgInFactory.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInFactory.o ${OBJECTDIR}/MsgInFactory_nomain.o;\
	fi

${OBJECTDIR}/ServerException_nomain.o: ${OBJECTDIR}/ServerException.o ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ServerException.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException_nomain.o ServerException.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ServerException.o ${OBJECTDIR}/ServerException_nomain.o;\
	fi

${OBJECTDIR}/SessionsTable_nomain.o: ${OBJECTDIR}/SessionsTable.o SessionsTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/SessionsTable.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SessionsTable_nomain.o SessionsTable.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/SessionsTable.o ${OBJECTDIR}/SessionsTable_nomain.o;\
	fi

${OBJECTDIR}/SocketClient_nomain.o: ${OBJECTDIR}/SocketClient.o SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/SocketClient.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient_nomain.o SocketClient.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/SocketClient.o ${OBJECTDIR}/SocketClient_nomain.o;\
	fi

${OBJECTDIR}/SocketServer_nomain.o: ${OBJECTDIR}/SocketServer.o SocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/SocketServer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer_nomain.o SocketServer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/SocketServer.o ${OBJECTDIR}/SocketServer_nomain.o;\
	fi

${OBJECTDIR}/TokenChecker_nomain.o: ${OBJECTDIR}/TokenChecker.o TokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/TokenChecker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TokenChecker_nomain.o TokenChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/TokenChecker.o ${OBJECTDIR}/TokenChecker_nomain.o;\
	fi

${OBJECTDIR}/communication_nomain.o: ${OBJECTDIR}/communication.o communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/communication.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication_nomain.o communication.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/communication.o ${OBJECTDIR}/communication_nomain.o;\
	fi

${OBJECTDIR}/fTokenChecker_nomain.o: ${OBJECTDIR}/fTokenChecker.o fTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/fTokenChecker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fTokenChecker_nomain.o fTokenChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/fTokenChecker.o ${OBJECTDIR}/fTokenChecker_nomain.o;\
	fi

${OBJECTDIR}/gTokenChecker_nomain.o: ${OBJECTDIR}/gTokenChecker.o gTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/gTokenChecker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker_nomain.o gTokenChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/gTokenChecker.o ${OBJECTDIR}/gTokenChecker_nomain.o;\
	fi

${OBJECTDIR}/msgs/AccountAdd_nomain.o: ${OBJECTDIR}/msgs/AccountAdd.o msgs/AccountAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/AccountAdd.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountAdd_nomain.o msgs/AccountAdd.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/AccountAdd.o ${OBJECTDIR}/msgs/AccountAdd_nomain.o;\
	fi

${OBJECTDIR}/msgs/AccountDel_nomain.o: ${OBJECTDIR}/msgs/AccountDel.o msgs/AccountDel.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/AccountDel.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountDel_nomain.o msgs/AccountDel.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/AccountDel.o ${OBJECTDIR}/msgs/AccountDel_nomain.o;\
	fi

${OBJECTDIR}/msgs/AccountGet_nomain.o: ${OBJECTDIR}/msgs/AccountGet.o msgs/AccountGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/AccountGet.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountGet_nomain.o msgs/AccountGet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/AccountGet.o ${OBJECTDIR}/msgs/AccountGet_nomain.o;\
	fi

${OBJECTDIR}/msgs/AccountUpdate_nomain.o: ${OBJECTDIR}/msgs/AccountUpdate.o msgs/AccountUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/AccountUpdate.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/AccountUpdate_nomain.o msgs/AccountUpdate.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/AccountUpdate.o ${OBJECTDIR}/msgs/AccountUpdate_nomain.o;\
	fi

${OBJECTDIR}/msgs/DeviceDelete_nomain.o: ${OBJECTDIR}/msgs/DeviceDelete.o msgs/DeviceDelete.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/DeviceDelete.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DeviceDelete_nomain.o msgs/DeviceDelete.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/DeviceDelete.o ${OBJECTDIR}/msgs/DeviceDelete_nomain.o;\
	fi

${OBJECTDIR}/msgs/DevicesGet_nomain.o: ${OBJECTDIR}/msgs/DevicesGet.o msgs/DevicesGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/DevicesGet.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesGet_nomain.o msgs/DevicesGet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/DevicesGet.o ${OBJECTDIR}/msgs/DevicesGet_nomain.o;\
	fi

${OBJECTDIR}/msgs/DevicesGetAll_nomain.o: ${OBJECTDIR}/msgs/DevicesGetAll.o msgs/DevicesGetAll.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/DevicesGetAll.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesGetAll_nomain.o msgs/DevicesGetAll.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/DevicesGetAll.o ${OBJECTDIR}/msgs/DevicesGetAll_nomain.o;\
	fi

${OBJECTDIR}/msgs/DevicesGetNew_nomain.o: ${OBJECTDIR}/msgs/DevicesGetNew.o msgs/DevicesGetNew.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/DevicesGetNew.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesGetNew_nomain.o msgs/DevicesGetNew.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/DevicesGetNew.o ${OBJECTDIR}/msgs/DevicesGetNew_nomain.o;\
	fi

${OBJECTDIR}/msgs/DevicesUpdate_nomain.o: ${OBJECTDIR}/msgs/DevicesUpdate.o msgs/DevicesUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/DevicesUpdate.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/DevicesUpdate_nomain.o msgs/DevicesUpdate.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/DevicesUpdate.o ${OBJECTDIR}/msgs/DevicesUpdate_nomain.o;\
	fi

${OBJECTDIR}/msgs/GateAdd_nomain.o: ${OBJECTDIR}/msgs/GateAdd.o msgs/GateAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GateAdd.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateAdd_nomain.o msgs/GateAdd.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/GateAdd.o ${OBJECTDIR}/msgs/GateAdd_nomain.o;\
	fi

${OBJECTDIR}/msgs/GateDelete_nomain.o: ${OBJECTDIR}/msgs/GateDelete.o msgs/GateDelete.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GateDelete.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateDelete_nomain.o msgs/GateDelete.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/GateDelete.o ${OBJECTDIR}/msgs/GateDelete_nomain.o;\
	fi

${OBJECTDIR}/msgs/GateGetInfo_nomain.o: ${OBJECTDIR}/msgs/GateGetInfo.o msgs/GateGetInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GateGetInfo.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateGetInfo_nomain.o msgs/GateGetInfo.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/GateGetInfo.o ${OBJECTDIR}/msgs/GateGetInfo_nomain.o;\
	fi

${OBJECTDIR}/msgs/GateScanMode_nomain.o: ${OBJECTDIR}/msgs/GateScanMode.o msgs/GateScanMode.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GateScanMode.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateScanMode_nomain.o msgs/GateScanMode.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/GateScanMode.o ${OBJECTDIR}/msgs/GateScanMode_nomain.o;\
	fi

${OBJECTDIR}/msgs/GateUpdate_nomain.o: ${OBJECTDIR}/msgs/GateUpdate.o msgs/GateUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GateUpdate.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateUpdate_nomain.o msgs/GateUpdate.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/GateUpdate.o ${OBJECTDIR}/msgs/GateUpdate_nomain.o;\
	fi

${OBJECTDIR}/msgs/GatesGetConnected_nomain.o: ${OBJECTDIR}/msgs/GatesGetConnected.o msgs/GatesGetConnected.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GatesGetConnected.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GatesGetConnected_nomain.o msgs/GatesGetConnected.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/GatesGetConnected.o ${OBJECTDIR}/msgs/GatesGetConnected_nomain.o;\
	fi

${OBJECTDIR}/msgs/IMsgIn_nomain.o: ${OBJECTDIR}/msgs/IMsgIn.o msgs/IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/IMsgIn.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgIn_nomain.o msgs/IMsgIn.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/IMsgIn.o ${OBJECTDIR}/msgs/IMsgIn_nomain.o;\
	fi

${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired_nomain.o: ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o msgs/IMsgInLoginAndAdapterAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired_nomain.o msgs/IMsgInLoginAndAdapterAccessRequired.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired_nomain.o;\
	fi

${OBJECTDIR}/msgs/IMsgInLoginRequired_nomain.o: ${OBJECTDIR}/msgs/IMsgInLoginRequired.o msgs/IMsgInLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/IMsgInLoginRequired.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginRequired_nomain.o msgs/IMsgInLoginRequired.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/IMsgInLoginRequired.o ${OBJECTDIR}/msgs/IMsgInLoginRequired_nomain.o;\
	fi

${OBJECTDIR}/msgs/IMsgInLoginUnwanted_nomain.o: ${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o msgs/IMsgInLoginUnwanted.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted_nomain.o msgs/IMsgInLoginUnwanted.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted_nomain.o;\
	fi

${OBJECTDIR}/msgs/LocationAdd_nomain.o: ${OBJECTDIR}/msgs/LocationAdd.o msgs/LocationAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/LocationAdd.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationAdd_nomain.o msgs/LocationAdd.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/LocationAdd.o ${OBJECTDIR}/msgs/LocationAdd_nomain.o;\
	fi

${OBJECTDIR}/msgs/LocationDelete_nomain.o: ${OBJECTDIR}/msgs/LocationDelete.o msgs/LocationDelete.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/LocationDelete.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationDelete_nomain.o msgs/LocationDelete.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/LocationDelete.o ${OBJECTDIR}/msgs/LocationDelete_nomain.o;\
	fi

${OBJECTDIR}/msgs/LocationsGet_nomain.o: ${OBJECTDIR}/msgs/LocationsGet.o msgs/LocationsGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/LocationsGet.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationsGet_nomain.o msgs/LocationsGet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/LocationsGet.o ${OBJECTDIR}/msgs/LocationsGet_nomain.o;\
	fi

${OBJECTDIR}/msgs/LocationsUpdate_nomain.o: ${OBJECTDIR}/msgs/LocationsUpdate.o msgs/LocationsUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/LocationsUpdate.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LocationsUpdate_nomain.o msgs/LocationsUpdate.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/LocationsUpdate.o ${OBJECTDIR}/msgs/LocationsUpdate_nomain.o;\
	fi

${OBJECTDIR}/msgs/LogGet_nomain.o: ${OBJECTDIR}/msgs/LogGet.o msgs/LogGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/LogGet.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/LogGet_nomain.o msgs/LogGet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/LogGet.o ${OBJECTDIR}/msgs/LogGet_nomain.o;\
	fi

${OBJECTDIR}/msgs/ModuleSwitchState_nomain.o: ${OBJECTDIR}/msgs/ModuleSwitchState.o msgs/ModuleSwitchState.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/ModuleSwitchState.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/ModuleSwitchState_nomain.o msgs/ModuleSwitchState.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/ModuleSwitchState.o ${OBJECTDIR}/msgs/ModuleSwitchState_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o: ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o msgs/MsgInAlgorithmsRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o msgs/MsgInAlgorithmsRedirect.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o: ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o msgs/MsgInGamificationRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o msgs/MsgInGamificationRedirect.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o ${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInUnknown_nomain.o: ${OBJECTDIR}/msgs/MsgInUnknown.o msgs/MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInUnknown.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUnknown_nomain.o msgs/MsgInUnknown.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInUnknown.o ${OBJECTDIR}/msgs/MsgInUnknown_nomain.o;\
	fi

${OBJECTDIR}/msgs/NotificationEraseGCMID_nomain.o: ${OBJECTDIR}/msgs/NotificationEraseGCMID.o msgs/NotificationEraseGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/NotificationEraseGCMID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationEraseGCMID_nomain.o msgs/NotificationEraseGCMID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/NotificationEraseGCMID.o ${OBJECTDIR}/msgs/NotificationEraseGCMID_nomain.o;\
	fi

${OBJECTDIR}/msgs/NotificationReaded_nomain.o: ${OBJECTDIR}/msgs/NotificationReaded.o msgs/NotificationReaded.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/NotificationReaded.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationReaded_nomain.o msgs/NotificationReaded.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/NotificationReaded.o ${OBJECTDIR}/msgs/NotificationReaded_nomain.o;\
	fi

${OBJECTDIR}/msgs/NotificationSetGCMID_nomain.o: ${OBJECTDIR}/msgs/NotificationSetGCMID.o msgs/NotificationSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/NotificationSetGCMID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationSetGCMID_nomain.o msgs/NotificationSetGCMID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/NotificationSetGCMID.o ${OBJECTDIR}/msgs/NotificationSetGCMID_nomain.o;\
	fi

${OBJECTDIR}/msgs/NotificationsGet_nomain.o: ${OBJECTDIR}/msgs/NotificationsGet.o msgs/NotificationsGet.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/NotificationsGet.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/NotificationsGet_nomain.o msgs/NotificationsGet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/NotificationsGet.o ${OBJECTDIR}/msgs/NotificationsGet_nomain.o;\
	fi

${OBJECTDIR}/msgs/UserGetID_nomain.o: ${OBJECTDIR}/msgs/UserGetID.o msgs/UserGetID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/UserGetID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetID_nomain.o msgs/UserGetID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/UserGetID.o ${OBJECTDIR}/msgs/UserGetID_nomain.o;\
	fi

${OBJECTDIR}/msgs/UserGetInfo_nomain.o: ${OBJECTDIR}/msgs/UserGetInfo.o msgs/UserGetInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/UserGetInfo.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetInfo_nomain.o msgs/UserGetInfo.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/UserGetInfo.o ${OBJECTDIR}/msgs/UserGetInfo_nomain.o;\
	fi

${OBJECTDIR}/msgs/UserLogIn_nomain.o: ${OBJECTDIR}/msgs/UserLogIn.o msgs/UserLogIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/UserLogIn.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogIn_nomain.o msgs/UserLogIn.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/UserLogIn.o ${OBJECTDIR}/msgs/UserLogIn_nomain.o;\
	fi

${OBJECTDIR}/msgs/UserLogout_nomain.o: ${OBJECTDIR}/msgs/UserLogout.o msgs/UserLogout.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/UserLogout.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogout_nomain.o msgs/UserLogout.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/UserLogout.o ${OBJECTDIR}/msgs/UserLogout_nomain.o;\
	fi

${OBJECTDIR}/msgs/UserRegister_nomain.o: ${OBJECTDIR}/msgs/UserRegister.o msgs/UserRegister.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/UserRegister.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserRegister_nomain.o msgs/UserRegister.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/UserRegister.o ${OBJECTDIR}/msgs/UserRegister_nomain.o;\
	fi

${OBJECTDIR}/save_custom_writer_nomain.o: ${OBJECTDIR}/save_custom_writer.o save_custom_writer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/save_custom_writer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer_nomain.o save_custom_writer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/save_custom_writer.o ${OBJECTDIR}/save_custom_writer_nomain.o;\
	fi

${OBJECTDIR}/sslServlet_nomain.o: ${OBJECTDIR}/sslServlet.o sslServlet.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/sslServlet.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet_nomain.o sslServlet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sslServlet.o ${OBJECTDIR}/sslServlet_nomain.o;\
	fi

${OBJECTDIR}/ui_server_nomain.o: ${OBJECTDIR}/ui_server.o ui_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ui_server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include -I. -I../Server -I/usr/include/soci -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server_nomain.o ui_server.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ui_server.o ${OBJECTDIR}/ui_server_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f21 || true; \
	    ${TESTDIR}/TestFiles/f15 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f20 || true; \
	    ${TESTDIR}/TestFiles/f8 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f31 || true; \
	    ${TESTDIR}/TestFiles/f16 || true; \
	    ${TESTDIR}/TestFiles/f9 || true; \
	    ${TESTDIR}/TestFiles/f26 || true; \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f19 || true; \
	    ${TESTDIR}/TestFiles/f29 || true; \
	    ${TESTDIR}/TestFiles/f28 || true; \
	    ${TESTDIR}/TestFiles/f18 || true; \
	    ${TESTDIR}/TestFiles/f17 || true; \
	    ${TESTDIR}/TestFiles/f24 || true; \
	    ${TESTDIR}/TestFiles/f23 || true; \
	    ${TESTDIR}/TestFiles/f7 || true; \
	    ${TESTDIR}/TestFiles/f14 || true; \
	    ${TESTDIR}/TestFiles/f11 || true; \
	    ${TESTDIR}/TestFiles/f27 || true; \
	    ${TESTDIR}/TestFiles/f22 || true; \
	    ${TESTDIR}/TestFiles/f13 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f10 || true; \
	    ${TESTDIR}/TestFiles/f12 || true; \
	    ${TESTDIR}/TestFiles/f30 || true; \
	    ${TESTDIR}/TestFiles/f25 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ./ui_server

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
