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
CND_CONF=Release
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
	${OBJECTDIR}/_ext/1154895860/Logger.o \
	${OBJECTDIR}/Config.o \
	${OBJECTDIR}/DBConnector.o \
	${OBJECTDIR}/MsgInFactory.o \
	${OBJECTDIR}/ServerException.o \
	${OBJECTDIR}/SessionsTable.o \
	${OBJECTDIR}/SocketClient.o \
	${OBJECTDIR}/SocketServer.o \
	${OBJECTDIR}/communication.o \
	${OBJECTDIR}/fTokenChecker.o \
	${OBJECTDIR}/gTokenChecker.o \
	${OBJECTDIR}/msgs/GateAdd.o \
	${OBJECTDIR}/msgs/GateDelete.o \
	${OBJECTDIR}/msgs/GateGetInfo.o \
	${OBJECTDIR}/msgs/GateUpdate.o \
	${OBJECTDIR}/msgs/GatesGetConnected.o \
	${OBJECTDIR}/msgs/IMsgIn.o \
	${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o \
	${OBJECTDIR}/msgs/IMsgInLoginRequired.o \
	${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o \
	${OBJECTDIR}/msgs/MsgInAdapterListen.o \
	${OBJECTDIR}/msgs/MsgInAddAccount.o \
	${OBJECTDIR}/msgs/MsgInAddRoom.o \
	${OBJECTDIR}/msgs/MsgInAddView.o \
	${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o \
	${OBJECTDIR}/msgs/MsgInDelConAccount.o \
	${OBJECTDIR}/msgs/MsgInDelDevice.o \
	${OBJECTDIR}/msgs/MsgInDelGCMID.o \
	${OBJECTDIR}/msgs/MsgInDelRoom.o \
	${OBJECTDIR}/msgs/MsgInDelView.o \
	${OBJECTDIR}/msgs/MsgInGamificationRedirect.o \
	${OBJECTDIR}/msgs/MsgInGetAllDevices.o \
	${OBJECTDIR}/msgs/MsgInGetConAccount.o \
	${OBJECTDIR}/msgs/MsgInGetDevices.o \
	${OBJECTDIR}/msgs/MsgInGetLog.o \
	${OBJECTDIR}/msgs/MsgInGetNewDevices.o \
	${OBJECTDIR}/msgs/MsgInGetNotifications.o \
	${OBJECTDIR}/msgs/MsgInGetRooms.o \
	${OBJECTDIR}/msgs/MsgInGetViews.o \
	${OBJECTDIR}/msgs/MsgInNotificationRead.o \
	${OBJECTDIR}/msgs/MsgInSetConAccount.o \
	${OBJECTDIR}/msgs/MsgInSetDevices.o \
	${OBJECTDIR}/msgs/MsgInSetGCMID.o \
	${OBJECTDIR}/msgs/MsgInSetRooms.o \
	${OBJECTDIR}/msgs/MsgInSwitch.o \
	${OBJECTDIR}/msgs/MsgInUnknown.o \
	${OBJECTDIR}/msgs/MsgInUpdateView.o \
	${OBJECTDIR}/msgs/UserGetID.o \
	${OBJECTDIR}/msgs/UserGetInfo.o \
	${OBJECTDIR}/msgs/UserLogIn.o \
	${OBJECTDIR}/msgs/UserLogout.o \
	${OBJECTDIR}/msgs/UserRegister.o \
	${OBJECTDIR}/pugixml.o \
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
CCFLAGS=`cppunit-config --cflags` 
CXXFLAGS=`cppunit-config --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../soci-3.2.2/build/lib -L/usr/local/include/soci -L../soci-3.2.2/soci-3.2.2/backends/postgresql -L../soci-3.2.2/soci-3.2.2/core -L/usr/include/postgresql -L/usr/include/postgresql/libpq -L. -L/usr/include -L/usr/local/include -L/usr/local/lib -L/usr/lib -Wl,-rpath,../soci-3.2.2/build/lib -Wl,-rpath,/usr/local/lib `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` -lpq `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ./ui_server

./ui_server: ${OBJECTFILES}
	${MKDIR} -p .
	${LINK.cc} -o ./ui_server ${OBJECTFILES} ${LDLIBSOPTIONS} -lpq -lsoci_core -lsoci_empty -lsoci_postgresql -lcppunit -lssl -lcrypto -ljansson

${OBJECTDIR}/_ext/1360890531/DAO.o: ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp

${OBJECTDIR}/_ext/1360890531/DAOAdapters.o: ../DAO/DAOAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters.o ../DAO/DAOAdapters.cpp

${OBJECTDIR}/_ext/1360890531/DAODevices.o: ../DAO/DAODevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices.o ../DAO/DAODevices.cpp

${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o: ../DAO/DAOMobileDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o ../DAO/DAOMobileDevices.cpp

${OBJECTDIR}/_ext/1360890531/DAONotification.o: ../DAO/DAONotification.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAONotification.o ../DAO/DAONotification.cpp

${OBJECTDIR}/_ext/1360890531/DAORooms.o: ../DAO/DAORooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms.o ../DAO/DAORooms.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsers.o: ../DAO/DAOUsers.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers.o ../DAO/DAOUsers.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o: ../DAO/DAOUsersAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o ../DAO/DAOUsersAdapters.cpp

${OBJECTDIR}/_ext/1154895860/Logger.o: ../ui_logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1154895860
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger.o ../ui_logger/Logger.cpp

${OBJECTDIR}/Config.o: Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config.o Config.cpp

${OBJECTDIR}/DBConnector.o: DBConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector.o DBConnector.cpp

${OBJECTDIR}/MsgInFactory.o: MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp

${OBJECTDIR}/ServerException.o: ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException.o ServerException.cpp

${OBJECTDIR}/SessionsTable.o: SessionsTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SessionsTable.o SessionsTable.cpp

${OBJECTDIR}/SocketClient.o: SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient.o SocketClient.cpp

${OBJECTDIR}/SocketServer.o: SocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer.o SocketServer.cpp

${OBJECTDIR}/communication.o: communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication.o communication.cpp

${OBJECTDIR}/fTokenChecker.o: fTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fTokenChecker.o fTokenChecker.cpp

${OBJECTDIR}/gTokenChecker.o: gTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker.o gTokenChecker.cpp

${OBJECTDIR}/msgs/GateAdd.o: msgs/GateAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateAdd.o msgs/GateAdd.cpp

${OBJECTDIR}/msgs/GateDelete.o: msgs/GateDelete.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateDelete.o msgs/GateDelete.cpp

${OBJECTDIR}/msgs/GateGetInfo.o: msgs/GateGetInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateGetInfo.o msgs/GateGetInfo.cpp

${OBJECTDIR}/msgs/GateUpdate.o: msgs/GateUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateUpdate.o msgs/GateUpdate.cpp

${OBJECTDIR}/msgs/GatesGetConnected.o: msgs/GatesGetConnected.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GatesGetConnected.o msgs/GatesGetConnected.cpp

${OBJECTDIR}/msgs/IMsgIn.o: msgs/IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgIn.o msgs/IMsgIn.cpp

${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o: msgs/IMsgInLoginAndAdapterAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o msgs/IMsgInLoginAndAdapterAccessRequired.cpp

${OBJECTDIR}/msgs/IMsgInLoginRequired.o: msgs/IMsgInLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginRequired.o msgs/IMsgInLoginRequired.cpp

${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o: msgs/IMsgInLoginUnwanted.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o msgs/IMsgInLoginUnwanted.cpp

${OBJECTDIR}/msgs/MsgInAdapterListen.o: msgs/MsgInAdapterListen.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAdapterListen.o msgs/MsgInAdapterListen.cpp

${OBJECTDIR}/msgs/MsgInAddAccount.o: msgs/MsgInAddAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddAccount.o msgs/MsgInAddAccount.cpp

${OBJECTDIR}/msgs/MsgInAddRoom.o: msgs/MsgInAddRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddRoom.o msgs/MsgInAddRoom.cpp

${OBJECTDIR}/msgs/MsgInAddView.o: msgs/MsgInAddView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddView.o msgs/MsgInAddView.cpp

${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o: msgs/MsgInAlgorithmsRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o msgs/MsgInAlgorithmsRedirect.cpp

${OBJECTDIR}/msgs/MsgInDelConAccount.o: msgs/MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelConAccount.o msgs/MsgInDelConAccount.cpp

${OBJECTDIR}/msgs/MsgInDelDevice.o: msgs/MsgInDelDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelDevice.o msgs/MsgInDelDevice.cpp

${OBJECTDIR}/msgs/MsgInDelGCMID.o: msgs/MsgInDelGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelGCMID.o msgs/MsgInDelGCMID.cpp

${OBJECTDIR}/msgs/MsgInDelRoom.o: msgs/MsgInDelRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelRoom.o msgs/MsgInDelRoom.cpp

${OBJECTDIR}/msgs/MsgInDelView.o: msgs/MsgInDelView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelView.o msgs/MsgInDelView.cpp

${OBJECTDIR}/msgs/MsgInGamificationRedirect.o: msgs/MsgInGamificationRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o msgs/MsgInGamificationRedirect.cpp

${OBJECTDIR}/msgs/MsgInGetAllDevices.o: msgs/MsgInGetAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetAllDevices.o msgs/MsgInGetAllDevices.cpp

${OBJECTDIR}/msgs/MsgInGetConAccount.o: msgs/MsgInGetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetConAccount.o msgs/MsgInGetConAccount.cpp

${OBJECTDIR}/msgs/MsgInGetDevices.o: msgs/MsgInGetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetDevices.o msgs/MsgInGetDevices.cpp

${OBJECTDIR}/msgs/MsgInGetLog.o: msgs/MsgInGetLog.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetLog.o msgs/MsgInGetLog.cpp

${OBJECTDIR}/msgs/MsgInGetNewDevices.o: msgs/MsgInGetNewDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNewDevices.o msgs/MsgInGetNewDevices.cpp

${OBJECTDIR}/msgs/MsgInGetNotifications.o: msgs/MsgInGetNotifications.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNotifications.o msgs/MsgInGetNotifications.cpp

${OBJECTDIR}/msgs/MsgInGetRooms.o: msgs/MsgInGetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetRooms.o msgs/MsgInGetRooms.cpp

${OBJECTDIR}/msgs/MsgInGetViews.o: msgs/MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetViews.o msgs/MsgInGetViews.cpp

${OBJECTDIR}/msgs/MsgInNotificationRead.o: msgs/MsgInNotificationRead.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInNotificationRead.o msgs/MsgInNotificationRead.cpp

${OBJECTDIR}/msgs/MsgInSetConAccount.o: msgs/MsgInSetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetConAccount.o msgs/MsgInSetConAccount.cpp

${OBJECTDIR}/msgs/MsgInSetDevices.o: msgs/MsgInSetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetDevices.o msgs/MsgInSetDevices.cpp

${OBJECTDIR}/msgs/MsgInSetGCMID.o: msgs/MsgInSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetGCMID.o msgs/MsgInSetGCMID.cpp

${OBJECTDIR}/msgs/MsgInSetRooms.o: msgs/MsgInSetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetRooms.o msgs/MsgInSetRooms.cpp

${OBJECTDIR}/msgs/MsgInSwitch.o: msgs/MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSwitch.o msgs/MsgInSwitch.cpp

${OBJECTDIR}/msgs/MsgInUnknown.o: msgs/MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUnknown.o msgs/MsgInUnknown.cpp

${OBJECTDIR}/msgs/MsgInUpdateView.o: msgs/MsgInUpdateView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUpdateView.o msgs/MsgInUpdateView.cpp

${OBJECTDIR}/msgs/UserGetID.o: msgs/UserGetID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetID.o msgs/UserGetID.cpp

${OBJECTDIR}/msgs/UserGetInfo.o: msgs/UserGetInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetInfo.o msgs/UserGetInfo.cpp

${OBJECTDIR}/msgs/UserLogIn.o: msgs/UserLogIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogIn.o msgs/UserLogIn.cpp

${OBJECTDIR}/msgs/UserLogout.o: msgs/UserLogout.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogout.o msgs/UserLogout.cpp

${OBJECTDIR}/msgs/UserRegister.o: msgs/UserRegister.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserRegister.o msgs/UserRegister.cpp

${OBJECTDIR}/pugixml.o: pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml.o pugixml.cpp

${OBJECTDIR}/save_custom_writer.o: save_custom_writer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer.o save_custom_writer.cpp

${OBJECTDIR}/sslServlet.o: sslServlet.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet.o sslServlet.cpp

${OBJECTDIR}/ui_server.o: ui_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server.o ui_server.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f21: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f21 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f15: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f15 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f20: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f20 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f8: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f8 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f4: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f31: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f31 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f16: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f16 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f9: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f9 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f26: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f26 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f5: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f19: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f19 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f29: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f29 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f28: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f28 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f18: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f18 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f17: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f17 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f24: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f24 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f23: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f23 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f7: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f14: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f14 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f11: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f11 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f27: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f27 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f22: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f22 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f13: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f13 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f1: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f2: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f10: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f10 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f12: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f12 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f30: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f30 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f25: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f25 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f6: ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${OBJECTDIR}/_ext/1360890531/DAO_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAO.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO_nomain.o ../DAO/DAO.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters_nomain.o ../DAO/DAOAdapters.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices_nomain.o ../DAO/DAODevices.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices_nomain.o ../DAO/DAOMobileDevices.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAONotification_nomain.o ../DAO/DAONotification.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms_nomain.o ../DAO/DAORooms.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers_nomain.o ../DAO/DAOUsers.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters_nomain.o ../DAO/DAOUsersAdapters.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters_nomain.o;\
	fi

${OBJECTDIR}/_ext/1154895860/Logger_nomain.o: ${OBJECTDIR}/_ext/1154895860/Logger.o ../ui_logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1154895860
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1154895860/Logger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger_nomain.o ../ui_logger/Logger.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config_nomain.o Config.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector_nomain.o DBConnector.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory_nomain.o MsgInFactory.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException_nomain.o ServerException.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SessionsTable_nomain.o SessionsTable.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient_nomain.o SocketClient.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer_nomain.o SocketServer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/SocketServer.o ${OBJECTDIR}/SocketServer_nomain.o;\
	fi

${OBJECTDIR}/communication_nomain.o: ${OBJECTDIR}/communication.o communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/communication.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication_nomain.o communication.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fTokenChecker_nomain.o fTokenChecker.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker_nomain.o gTokenChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/gTokenChecker.o ${OBJECTDIR}/gTokenChecker_nomain.o;\
	fi

${OBJECTDIR}/msgs/GateAdd_nomain.o: ${OBJECTDIR}/msgs/GateAdd.o msgs/GateAdd.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GateAdd.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateAdd_nomain.o msgs/GateAdd.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateDelete_nomain.o msgs/GateDelete.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateGetInfo_nomain.o msgs/GateGetInfo.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/GateGetInfo.o ${OBJECTDIR}/msgs/GateGetInfo_nomain.o;\
	fi

${OBJECTDIR}/msgs/GateUpdate_nomain.o: ${OBJECTDIR}/msgs/GateUpdate.o msgs/GateUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/GateUpdate.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GateUpdate_nomain.o msgs/GateUpdate.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/GatesGetConnected_nomain.o msgs/GatesGetConnected.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgIn_nomain.o msgs/IMsgIn.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired_nomain.o msgs/IMsgInLoginAndAdapterAccessRequired.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginRequired_nomain.o msgs/IMsgInLoginRequired.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted_nomain.o msgs/IMsgInLoginUnwanted.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAdapterListen_nomain.o: ${OBJECTDIR}/msgs/MsgInAdapterListen.o msgs/MsgInAdapterListen.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAdapterListen.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAdapterListen_nomain.o msgs/MsgInAdapterListen.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAdapterListen.o ${OBJECTDIR}/msgs/MsgInAdapterListen_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAddAccount_nomain.o: ${OBJECTDIR}/msgs/MsgInAddAccount.o msgs/MsgInAddAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAddAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddAccount_nomain.o msgs/MsgInAddAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAddAccount.o ${OBJECTDIR}/msgs/MsgInAddAccount_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAddRoom_nomain.o: ${OBJECTDIR}/msgs/MsgInAddRoom.o msgs/MsgInAddRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAddRoom.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddRoom_nomain.o msgs/MsgInAddRoom.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAddRoom.o ${OBJECTDIR}/msgs/MsgInAddRoom_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAddView_nomain.o: ${OBJECTDIR}/msgs/MsgInAddView.o msgs/MsgInAddView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAddView.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddView_nomain.o msgs/MsgInAddView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAddView.o ${OBJECTDIR}/msgs/MsgInAddView_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o: ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o msgs/MsgInAlgorithmsRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o msgs/MsgInAlgorithmsRedirect.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInDelConAccount_nomain.o: ${OBJECTDIR}/msgs/MsgInDelConAccount.o msgs/MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInDelConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelConAccount_nomain.o msgs/MsgInDelConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInDelConAccount.o ${OBJECTDIR}/msgs/MsgInDelConAccount_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInDelDevice_nomain.o: ${OBJECTDIR}/msgs/MsgInDelDevice.o msgs/MsgInDelDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInDelDevice.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelDevice_nomain.o msgs/MsgInDelDevice.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInDelDevice.o ${OBJECTDIR}/msgs/MsgInDelDevice_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInDelGCMID_nomain.o: ${OBJECTDIR}/msgs/MsgInDelGCMID.o msgs/MsgInDelGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInDelGCMID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelGCMID_nomain.o msgs/MsgInDelGCMID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInDelGCMID.o ${OBJECTDIR}/msgs/MsgInDelGCMID_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInDelRoom_nomain.o: ${OBJECTDIR}/msgs/MsgInDelRoom.o msgs/MsgInDelRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInDelRoom.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelRoom_nomain.o msgs/MsgInDelRoom.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInDelRoom.o ${OBJECTDIR}/msgs/MsgInDelRoom_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInDelView_nomain.o: ${OBJECTDIR}/msgs/MsgInDelView.o msgs/MsgInDelView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInDelView.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelView_nomain.o msgs/MsgInDelView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInDelView.o ${OBJECTDIR}/msgs/MsgInDelView_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o: ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o msgs/MsgInGamificationRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o msgs/MsgInGamificationRedirect.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o ${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetAllDevices_nomain.o: ${OBJECTDIR}/msgs/MsgInGetAllDevices.o msgs/MsgInGetAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetAllDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetAllDevices_nomain.o msgs/MsgInGetAllDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetAllDevices.o ${OBJECTDIR}/msgs/MsgInGetAllDevices_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetConAccount_nomain.o: ${OBJECTDIR}/msgs/MsgInGetConAccount.o msgs/MsgInGetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetConAccount_nomain.o msgs/MsgInGetConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetConAccount.o ${OBJECTDIR}/msgs/MsgInGetConAccount_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetDevices_nomain.o: ${OBJECTDIR}/msgs/MsgInGetDevices.o msgs/MsgInGetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetDevices_nomain.o msgs/MsgInGetDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetDevices.o ${OBJECTDIR}/msgs/MsgInGetDevices_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetLog_nomain.o: ${OBJECTDIR}/msgs/MsgInGetLog.o msgs/MsgInGetLog.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetLog.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetLog_nomain.o msgs/MsgInGetLog.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetLog.o ${OBJECTDIR}/msgs/MsgInGetLog_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetNewDevices_nomain.o: ${OBJECTDIR}/msgs/MsgInGetNewDevices.o msgs/MsgInGetNewDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetNewDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNewDevices_nomain.o msgs/MsgInGetNewDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetNewDevices.o ${OBJECTDIR}/msgs/MsgInGetNewDevices_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetNotifications_nomain.o: ${OBJECTDIR}/msgs/MsgInGetNotifications.o msgs/MsgInGetNotifications.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetNotifications.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNotifications_nomain.o msgs/MsgInGetNotifications.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetNotifications.o ${OBJECTDIR}/msgs/MsgInGetNotifications_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetRooms_nomain.o: ${OBJECTDIR}/msgs/MsgInGetRooms.o msgs/MsgInGetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetRooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetRooms_nomain.o msgs/MsgInGetRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetRooms.o ${OBJECTDIR}/msgs/MsgInGetRooms_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetViews_nomain.o: ${OBJECTDIR}/msgs/MsgInGetViews.o msgs/MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetViews.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetViews_nomain.o msgs/MsgInGetViews.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetViews.o ${OBJECTDIR}/msgs/MsgInGetViews_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInNotificationRead_nomain.o: ${OBJECTDIR}/msgs/MsgInNotificationRead.o msgs/MsgInNotificationRead.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInNotificationRead.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInNotificationRead_nomain.o msgs/MsgInNotificationRead.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInNotificationRead.o ${OBJECTDIR}/msgs/MsgInNotificationRead_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSetConAccount_nomain.o: ${OBJECTDIR}/msgs/MsgInSetConAccount.o msgs/MsgInSetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSetConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetConAccount_nomain.o msgs/MsgInSetConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSetConAccount.o ${OBJECTDIR}/msgs/MsgInSetConAccount_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSetDevices_nomain.o: ${OBJECTDIR}/msgs/MsgInSetDevices.o msgs/MsgInSetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSetDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetDevices_nomain.o msgs/MsgInSetDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSetDevices.o ${OBJECTDIR}/msgs/MsgInSetDevices_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSetGCMID_nomain.o: ${OBJECTDIR}/msgs/MsgInSetGCMID.o msgs/MsgInSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSetGCMID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetGCMID_nomain.o msgs/MsgInSetGCMID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSetGCMID.o ${OBJECTDIR}/msgs/MsgInSetGCMID_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSetRooms_nomain.o: ${OBJECTDIR}/msgs/MsgInSetRooms.o msgs/MsgInSetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSetRooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetRooms_nomain.o msgs/MsgInSetRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSetRooms.o ${OBJECTDIR}/msgs/MsgInSetRooms_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSwitch_nomain.o: ${OBJECTDIR}/msgs/MsgInSwitch.o msgs/MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSwitch.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSwitch_nomain.o msgs/MsgInSwitch.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSwitch.o ${OBJECTDIR}/msgs/MsgInSwitch_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInUnknown_nomain.o: ${OBJECTDIR}/msgs/MsgInUnknown.o msgs/MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInUnknown.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUnknown_nomain.o msgs/MsgInUnknown.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInUnknown.o ${OBJECTDIR}/msgs/MsgInUnknown_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInUpdateView_nomain.o: ${OBJECTDIR}/msgs/MsgInUpdateView.o msgs/MsgInUpdateView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInUpdateView.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUpdateView_nomain.o msgs/MsgInUpdateView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInUpdateView.o ${OBJECTDIR}/msgs/MsgInUpdateView_nomain.o;\
	fi

${OBJECTDIR}/msgs/UserGetID_nomain.o: ${OBJECTDIR}/msgs/UserGetID.o msgs/UserGetID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/UserGetID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetID_nomain.o msgs/UserGetID.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserGetInfo_nomain.o msgs/UserGetInfo.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogIn_nomain.o msgs/UserLogIn.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserLogout_nomain.o msgs/UserLogout.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/UserRegister_nomain.o msgs/UserRegister.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/UserRegister.o ${OBJECTDIR}/msgs/UserRegister_nomain.o;\
	fi

${OBJECTDIR}/pugixml_nomain.o: ${OBJECTDIR}/pugixml.o pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/pugixml.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml_nomain.o pugixml.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/pugixml.o ${OBJECTDIR}/pugixml_nomain.o;\
	fi

${OBJECTDIR}/save_custom_writer_nomain.o: ${OBJECTDIR}/save_custom_writer.o save_custom_writer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/save_custom_writer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer_nomain.o save_custom_writer.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet_nomain.o sslServlet.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -I. -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server_nomain.o ui_server.cpp;\
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
