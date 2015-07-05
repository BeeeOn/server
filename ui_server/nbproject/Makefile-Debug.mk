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
	${OBJECTDIR}/_ext/1154895860/Logger.o \
	${OBJECTDIR}/Config.o \
	${OBJECTDIR}/DBConnector.o \
	${OBJECTDIR}/MsgInFactory.o \
	${OBJECTDIR}/MsgRightsChecker.o \
	${OBJECTDIR}/ServerException.o \
	${OBJECTDIR}/SocketClient.o \
	${OBJECTDIR}/SocketServer.o \
	${OBJECTDIR}/communication.o \
	${OBJECTDIR}/fTokenChecker.o \
	${OBJECTDIR}/gTokenChecker.o \
	${OBJECTDIR}/msgs/IMsgIn.o \
	${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o \
	${OBJECTDIR}/msgs/IMsgInLoginRequired.o \
	${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o \
	${OBJECTDIR}/msgs/MsgInAdapterListen.o \
	${OBJECTDIR}/msgs/MsgInAddAccount.o \
	${OBJECTDIR}/msgs/MsgInAddAdapter.o \
	${OBJECTDIR}/msgs/MsgInAddRoom.o \
	${OBJECTDIR}/msgs/MsgInAddView.o \
	${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o \
	${OBJECTDIR}/msgs/MsgInDelAdapter.o \
	${OBJECTDIR}/msgs/MsgInDelConAccount.o \
	${OBJECTDIR}/msgs/MsgInDelDevice.o \
	${OBJECTDIR}/msgs/MsgInDelGCMID.o \
	${OBJECTDIR}/msgs/MsgInDelRoom.o \
	${OBJECTDIR}/msgs/MsgInDelView.o \
	${OBJECTDIR}/msgs/MsgInGamificationRedirect.o \
	${OBJECTDIR}/msgs/MsgInGetAdapters.o \
	${OBJECTDIR}/msgs/MsgInGetAllDevices.o \
	${OBJECTDIR}/msgs/MsgInGetConAccount.o \
	${OBJECTDIR}/msgs/MsgInGetDevices.o \
	${OBJECTDIR}/msgs/MsgInGetNewDevices.o \
	${OBJECTDIR}/msgs/MsgInGetNotifications.o \
	${OBJECTDIR}/msgs/MsgInGetRooms.o \
	${OBJECTDIR}/msgs/MsgInGetTimeZone.o \
	${OBJECTDIR}/msgs/MsgInGetUserID.o \
	${OBJECTDIR}/msgs/MsgInGetUserInfo.o \
	${OBJECTDIR}/msgs/MsgInGetViews.o \
	${OBJECTDIR}/msgs/MsgInNotificationRead.o \
	${OBJECTDIR}/msgs/MsgInSetConAccount.o \
	${OBJECTDIR}/msgs/MsgInSetGCMID.o \
	${OBJECTDIR}/msgs/MsgInSetRooms.o \
	${OBJECTDIR}/msgs/MsgInSetTimeZone.o \
	${OBJECTDIR}/msgs/MsgInSignMe.o \
	${OBJECTDIR}/msgs/MsgInSignUp.o \
	${OBJECTDIR}/msgs/MsgInSwitch.o \
	${OBJECTDIR}/msgs/MsgInUnknown.o \
	${OBJECTDIR}/msgs/MsgInUpdateView.o \
	${OBJECTDIR}/pugixml.o \
	${OBJECTDIR}/save_custom_writer.o \
	${OBJECTDIR}/sslServlet.o \
	${OBJECTDIR}/testHelper.o \
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
LDLIBSOPTIONS=-L/usr/local/lib -L/usr/local/lib64 -L/opt/centos/devtoolset-1.1/root/usr/lib/gcc/x86_64-redhat-linux/4.7.2 -L/usr/pgsql-9.2/lib -L/usr/lib64 -Wl,-rpath,/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -lpq -ljansson `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ./ui_server

./ui_server: ${OBJECTFILES}
	${MKDIR} -p .
	g++ -o ./ui_server ${OBJECTFILES} ${LDLIBSOPTIONS} -lpq -lsoci_core -lsoci_empty -lsoci_postgresql -lcurl -lssl -lcrypto -ljansson -lstdc++

${OBJECTDIR}/_ext/1360890531/DAO.o: ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp

${OBJECTDIR}/_ext/1360890531/DAOAdapters.o: ../DAO/DAOAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters.o ../DAO/DAOAdapters.cpp

${OBJECTDIR}/_ext/1360890531/DAODevices.o: ../DAO/DAODevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices.o ../DAO/DAODevices.cpp

${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o: ../DAO/DAOMobileDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o ../DAO/DAOMobileDevices.cpp

${OBJECTDIR}/_ext/1360890531/DAONotification.o: ../DAO/DAONotification.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAONotification.o ../DAO/DAONotification.cpp

${OBJECTDIR}/_ext/1360890531/DAORooms.o: ../DAO/DAORooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms.o ../DAO/DAORooms.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsers.o: ../DAO/DAOUsers.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers.o ../DAO/DAOUsers.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o: ../DAO/DAOUsersAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o ../DAO/DAOUsersAdapters.cpp

${OBJECTDIR}/_ext/1154895860/Logger.o: ../ui_logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1154895860
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger.o ../ui_logger/Logger.cpp

${OBJECTDIR}/Config.o: Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config.o Config.cpp

${OBJECTDIR}/DBConnector.o: DBConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector.o DBConnector.cpp

${OBJECTDIR}/MsgInFactory.o: MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp

${OBJECTDIR}/MsgRightsChecker.o: MsgRightsChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgRightsChecker.o MsgRightsChecker.cpp

${OBJECTDIR}/ServerException.o: ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException.o ServerException.cpp

${OBJECTDIR}/SocketClient.o: SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient.o SocketClient.cpp

${OBJECTDIR}/SocketServer.o: SocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer.o SocketServer.cpp

${OBJECTDIR}/communication.o: communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication.o communication.cpp

${OBJECTDIR}/fTokenChecker.o: fTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fTokenChecker.o fTokenChecker.cpp

${OBJECTDIR}/gTokenChecker.o: gTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker.o gTokenChecker.cpp

${OBJECTDIR}/msgs/IMsgIn.o: msgs/IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgIn.o msgs/IMsgIn.cpp

${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o: msgs/IMsgInLoginAndAdapterAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired.o msgs/IMsgInLoginAndAdapterAccessRequired.cpp

${OBJECTDIR}/msgs/IMsgInLoginRequired.o: msgs/IMsgInLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginRequired.o msgs/IMsgInLoginRequired.cpp

${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o: msgs/IMsgInLoginUnwanted.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted.o msgs/IMsgInLoginUnwanted.cpp

${OBJECTDIR}/msgs/MsgInAdapterListen.o: msgs/MsgInAdapterListen.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAdapterListen.o msgs/MsgInAdapterListen.cpp

${OBJECTDIR}/msgs/MsgInAddAccount.o: msgs/MsgInAddAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddAccount.o msgs/MsgInAddAccount.cpp

${OBJECTDIR}/msgs/MsgInAddAdapter.o: msgs/MsgInAddAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddAdapter.o msgs/MsgInAddAdapter.cpp

${OBJECTDIR}/msgs/MsgInAddRoom.o: msgs/MsgInAddRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddRoom.o msgs/MsgInAddRoom.cpp

${OBJECTDIR}/msgs/MsgInAddView.o: msgs/MsgInAddView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddView.o msgs/MsgInAddView.cpp

${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o: msgs/MsgInAlgorithmsRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o msgs/MsgInAlgorithmsRedirect.cpp

${OBJECTDIR}/msgs/MsgInDelAdapter.o: msgs/MsgInDelAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelAdapter.o msgs/MsgInDelAdapter.cpp

${OBJECTDIR}/msgs/MsgInDelConAccount.o: msgs/MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelConAccount.o msgs/MsgInDelConAccount.cpp

${OBJECTDIR}/msgs/MsgInDelDevice.o: msgs/MsgInDelDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelDevice.o msgs/MsgInDelDevice.cpp

${OBJECTDIR}/msgs/MsgInDelGCMID.o: msgs/MsgInDelGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelGCMID.o msgs/MsgInDelGCMID.cpp

${OBJECTDIR}/msgs/MsgInDelRoom.o: msgs/MsgInDelRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelRoom.o msgs/MsgInDelRoom.cpp

${OBJECTDIR}/msgs/MsgInDelView.o: msgs/MsgInDelView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelView.o msgs/MsgInDelView.cpp

${OBJECTDIR}/msgs/MsgInGamificationRedirect.o: msgs/MsgInGamificationRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o msgs/MsgInGamificationRedirect.cpp

${OBJECTDIR}/msgs/MsgInGetAdapters.o: msgs/MsgInGetAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetAdapters.o msgs/MsgInGetAdapters.cpp

${OBJECTDIR}/msgs/MsgInGetAllDevices.o: msgs/MsgInGetAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetAllDevices.o msgs/MsgInGetAllDevices.cpp

${OBJECTDIR}/msgs/MsgInGetConAccount.o: msgs/MsgInGetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetConAccount.o msgs/MsgInGetConAccount.cpp

${OBJECTDIR}/msgs/MsgInGetDevices.o: msgs/MsgInGetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetDevices.o msgs/MsgInGetDevices.cpp

${OBJECTDIR}/msgs/MsgInGetNewDevices.o: msgs/MsgInGetNewDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNewDevices.o msgs/MsgInGetNewDevices.cpp

${OBJECTDIR}/msgs/MsgInGetNotifications.o: msgs/MsgInGetNotifications.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNotifications.o msgs/MsgInGetNotifications.cpp

${OBJECTDIR}/msgs/MsgInGetRooms.o: msgs/MsgInGetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetRooms.o msgs/MsgInGetRooms.cpp

${OBJECTDIR}/msgs/MsgInGetTimeZone.o: msgs/MsgInGetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetTimeZone.o msgs/MsgInGetTimeZone.cpp

${OBJECTDIR}/msgs/MsgInGetUserID.o: msgs/MsgInGetUserID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetUserID.o msgs/MsgInGetUserID.cpp

${OBJECTDIR}/msgs/MsgInGetUserInfo.o: msgs/MsgInGetUserInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetUserInfo.o msgs/MsgInGetUserInfo.cpp

${OBJECTDIR}/msgs/MsgInGetViews.o: msgs/MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetViews.o msgs/MsgInGetViews.cpp

${OBJECTDIR}/msgs/MsgInNotificationRead.o: msgs/MsgInNotificationRead.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInNotificationRead.o msgs/MsgInNotificationRead.cpp

${OBJECTDIR}/msgs/MsgInSetConAccount.o: msgs/MsgInSetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetConAccount.o msgs/MsgInSetConAccount.cpp

${OBJECTDIR}/msgs/MsgInSetGCMID.o: msgs/MsgInSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetGCMID.o msgs/MsgInSetGCMID.cpp

${OBJECTDIR}/msgs/MsgInSetRooms.o: msgs/MsgInSetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetRooms.o msgs/MsgInSetRooms.cpp

${OBJECTDIR}/msgs/MsgInSetTimeZone.o: msgs/MsgInSetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetTimeZone.o msgs/MsgInSetTimeZone.cpp

${OBJECTDIR}/msgs/MsgInSignMe.o: msgs/MsgInSignMe.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSignMe.o msgs/MsgInSignMe.cpp

${OBJECTDIR}/msgs/MsgInSignUp.o: msgs/MsgInSignUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSignUp.o msgs/MsgInSignUp.cpp

${OBJECTDIR}/msgs/MsgInSwitch.o: msgs/MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSwitch.o msgs/MsgInSwitch.cpp

${OBJECTDIR}/msgs/MsgInUnknown.o: msgs/MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUnknown.o msgs/MsgInUnknown.cpp

${OBJECTDIR}/msgs/MsgInUpdateView.o: msgs/MsgInUpdateView.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUpdateView.o msgs/MsgInUpdateView.cpp

${OBJECTDIR}/pugixml.o: pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml.o pugixml.cpp

${OBJECTDIR}/save_custom_writer.o: save_custom_writer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer.o save_custom_writer.cpp

${OBJECTDIR}/sslServlet.o: sslServlet.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet.o sslServlet.cpp

${OBJECTDIR}/testHelper.o: testHelper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/testHelper.o testHelper.cpp

${OBJECTDIR}/ui_server.o: ui_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server.o ui_server.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f21: ${TESTDIR}/tests/newtestclass.o ${TESTDIR}/tests/newtestrunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f21 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f15: ${TESTDIR}/tests/testChangeConAccount.o ${TESTDIR}/tests/testrunnerChangeConAccount.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f15 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/testGlobal.o ${TESTDIR}/tests/testrunnerGlobal.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f20: ${TESTDIR}/tests/testMsgInAddAdapter.o ${TESTDIR}/tests/testrunnerMsgInAddAdapter.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f20 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f8: ${TESTDIR}/tests/testMsgInAddConAcc.o ${TESTDIR}/tests/testrunnerMsgInAddConAcc.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f8 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/testMsgInAddRooms.o ${TESTDIR}/tests/testrunnerMsgInAddRooms.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f31: ${TESTDIR}/tests/testMsgInDelAction.o ${TESTDIR}/tests/testrunnerMsgInDelAction.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f31 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f16: ${TESTDIR}/tests/testMsgInDelConAcc.o ${TESTDIR}/tests/testrunnerMsgInDelConAcc.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f16 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f9: ${TESTDIR}/tests/testMsgInDelDevice.o ${TESTDIR}/tests/testrunnerMsgInDelDevice.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f9 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f26: ${TESTDIR}/tests/testMsgInDeleteCondition.o ${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f26 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/testMsgInDelRoom.o ${TESTDIR}/tests/testrunnerMsgInDelRoom.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f19: ${TESTDIR}/tests/testMsgInPartial.o ${TESTDIR}/tests/testrunnerMsgInPartial.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f19 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f29: ${TESTDIR}/tests/testMsgInGetAction.o ${TESTDIR}/tests/testrunnerMsgInGetAction.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f29 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f28: ${TESTDIR}/tests/testMsgInGetActions.o ${TESTDIR}/tests/testrunnerMsgInGetActions.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f28 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f18: ${TESTDIR}/tests/testMsgInGetAdapters.o ${TESTDIR}/tests/testrunnerMsgInGetAdapters.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f18 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f17: ${TESTDIR}/tests/testMsgInGetConAcc.o ${TESTDIR}/tests/testrunnerMsgInGetConAcc.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f17 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f24: ${TESTDIR}/tests/testMsgInGetConditions.o ${TESTDIR}/tests/testrunnerMsgInGetConditions.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f24 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f23: ${TESTDIR}/tests/testMsgInGetDevs.o ${TESTDIR}/tests/testrunnerMsgInGetDevs.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f23 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f7: ${TESTDIR}/tests/testMsgInGetRooms.o ${TESTDIR}/tests/testrunnerMsgInGetRooms.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f14: ${TESTDIR}/tests/testMsgInGetTimeZone.o ${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f14 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f11: ${TESTDIR}/tests/testMsgInLogName.o ${TESTDIR}/tests/testrunnerMsgInLogName.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f11 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f27: ${TESTDIR}/tests/testMsgInSetAction.o ${TESTDIR}/tests/testrunnerMsgInSetAction.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f27 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f22: ${TESTDIR}/tests/testMsgInSetCondition.o ${TESTDIR}/tests/testrunnerMsgInSetCondition.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f22 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f13: ${TESTDIR}/tests/testMsgInSetTimeZone.o ${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f13 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/testMsgInSignIn.o ${TESTDIR}/tests/testrunnerMsgInSignIn.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/testMsgInSignUp.o ${TESTDIR}/tests/testrunnerMsgInSignUp.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f10: ${TESTDIR}/tests/testMsgInSwitch.o ${TESTDIR}/tests/testrunnerMsgInSwitch.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f10 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f12: ${TESTDIR}/tests/testMsgInUpdate.o ${TESTDIR}/tests/testrunnerMsgInUpdate.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f12 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f30: ${TESTDIR}/tests/testMsgInUpdateAction.o ${TESTDIR}/tests/testrunnerMsgInUpdateAction.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f30 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f25: ${TESTDIR}/tests/testMsgInUpdateCondition.o ${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f25 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests/testMsgInUpdateRooms.o ${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} -L/usr/local/lib64 -L/usr/local/lib -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib -lsoci_core `cppunit-config --libs`   


${TESTDIR}/tests/newtestclass.o: tests/newtestclass.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestclass.o tests/newtestclass.cpp


${TESTDIR}/tests/newtestrunner.o: tests/newtestrunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestrunner.o tests/newtestrunner.cpp


${TESTDIR}/tests/testChangeConAccount.o: tests/testChangeConAccount.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testChangeConAccount.o tests/testChangeConAccount.cpp


${TESTDIR}/tests/testrunnerChangeConAccount.o: tests/testrunnerChangeConAccount.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerChangeConAccount.o tests/testrunnerChangeConAccount.cpp


${TESTDIR}/tests/testGlobal.o: tests/testGlobal.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testGlobal.o tests/testGlobal.cpp


${TESTDIR}/tests/testrunnerGlobal.o: tests/testrunnerGlobal.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerGlobal.o tests/testrunnerGlobal.cpp


${TESTDIR}/tests/testMsgInAddAdapter.o: tests/testMsgInAddAdapter.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddAdapter.o tests/testMsgInAddAdapter.cpp


${TESTDIR}/tests/testrunnerMsgInAddAdapter.o: tests/testrunnerMsgInAddAdapter.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddAdapter.o tests/testrunnerMsgInAddAdapter.cpp


${TESTDIR}/tests/testMsgInAddConAcc.o: tests/testMsgInAddConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddConAcc.o tests/testMsgInAddConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInAddConAcc.o: tests/testrunnerMsgInAddConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddConAcc.o tests/testrunnerMsgInAddConAcc.cpp


${TESTDIR}/tests/testMsgInAddRooms.o: tests/testMsgInAddRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddRooms.o tests/testMsgInAddRooms.cpp


${TESTDIR}/tests/testrunnerMsgInAddRooms.o: tests/testrunnerMsgInAddRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddRooms.o tests/testrunnerMsgInAddRooms.cpp


${TESTDIR}/tests/testMsgInDelAction.o: tests/testMsgInDelAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelAction.o tests/testMsgInDelAction.cpp


${TESTDIR}/tests/testrunnerMsgInDelAction.o: tests/testrunnerMsgInDelAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelAction.o tests/testrunnerMsgInDelAction.cpp


${TESTDIR}/tests/testMsgInDelConAcc.o: tests/testMsgInDelConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelConAcc.o tests/testMsgInDelConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInDelConAcc.o: tests/testrunnerMsgInDelConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelConAcc.o tests/testrunnerMsgInDelConAcc.cpp


${TESTDIR}/tests/testMsgInDelDevice.o: tests/testMsgInDelDevice.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelDevice.o tests/testMsgInDelDevice.cpp


${TESTDIR}/tests/testrunnerMsgInDelDevice.o: tests/testrunnerMsgInDelDevice.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelDevice.o tests/testrunnerMsgInDelDevice.cpp


${TESTDIR}/tests/testMsgInDeleteCondition.o: tests/testMsgInDeleteCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDeleteCondition.o tests/testMsgInDeleteCondition.cpp


${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o: tests/testrunnerMsgInDeleteCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o tests/testrunnerMsgInDeleteCondition.cpp


${TESTDIR}/tests/testMsgInDelRoom.o: tests/testMsgInDelRoom.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelRoom.o tests/testMsgInDelRoom.cpp


${TESTDIR}/tests/testrunnerMsgInDelRoom.o: tests/testrunnerMsgInDelRoom.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelRoom.o tests/testrunnerMsgInDelRoom.cpp


${TESTDIR}/tests/testMsgInPartial.o: tests/testMsgInPartial.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInPartial.o tests/testMsgInPartial.cpp


${TESTDIR}/tests/testrunnerMsgInPartial.o: tests/testrunnerMsgInPartial.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInPartial.o tests/testrunnerMsgInPartial.cpp


${TESTDIR}/tests/testMsgInGetAction.o: tests/testMsgInGetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetAction.o tests/testMsgInGetAction.cpp


${TESTDIR}/tests/testrunnerMsgInGetAction.o: tests/testrunnerMsgInGetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetAction.o tests/testrunnerMsgInGetAction.cpp


${TESTDIR}/tests/testMsgInGetActions.o: tests/testMsgInGetActions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetActions.o tests/testMsgInGetActions.cpp


${TESTDIR}/tests/testrunnerMsgInGetActions.o: tests/testrunnerMsgInGetActions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetActions.o tests/testrunnerMsgInGetActions.cpp


${TESTDIR}/tests/testMsgInGetAdapters.o: tests/testMsgInGetAdapters.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetAdapters.o tests/testMsgInGetAdapters.cpp


${TESTDIR}/tests/testrunnerMsgInGetAdapters.o: tests/testrunnerMsgInGetAdapters.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetAdapters.o tests/testrunnerMsgInGetAdapters.cpp


${TESTDIR}/tests/testMsgInGetConAcc.o: tests/testMsgInGetConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetConAcc.o tests/testMsgInGetConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInGetConAcc.o: tests/testrunnerMsgInGetConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetConAcc.o tests/testrunnerMsgInGetConAcc.cpp


${TESTDIR}/tests/testMsgInGetConditions.o: tests/testMsgInGetConditions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetConditions.o tests/testMsgInGetConditions.cpp


${TESTDIR}/tests/testrunnerMsgInGetConditions.o: tests/testrunnerMsgInGetConditions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetConditions.o tests/testrunnerMsgInGetConditions.cpp


${TESTDIR}/tests/testMsgInGetDevs.o: tests/testMsgInGetDevs.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetDevs.o tests/testMsgInGetDevs.cpp


${TESTDIR}/tests/testrunnerMsgInGetDevs.o: tests/testrunnerMsgInGetDevs.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetDevs.o tests/testrunnerMsgInGetDevs.cpp


${TESTDIR}/tests/testMsgInGetRooms.o: tests/testMsgInGetRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetRooms.o tests/testMsgInGetRooms.cpp


${TESTDIR}/tests/testrunnerMsgInGetRooms.o: tests/testrunnerMsgInGetRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetRooms.o tests/testrunnerMsgInGetRooms.cpp


${TESTDIR}/tests/testMsgInGetTimeZone.o: tests/testMsgInGetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetTimeZone.o tests/testMsgInGetTimeZone.cpp


${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o: tests/testrunnerMsgInGetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o tests/testrunnerMsgInGetTimeZone.cpp


${TESTDIR}/tests/testMsgInLogName.o: tests/testMsgInLogName.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInLogName.o tests/testMsgInLogName.cpp


${TESTDIR}/tests/testrunnerMsgInLogName.o: tests/testrunnerMsgInLogName.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInLogName.o tests/testrunnerMsgInLogName.cpp


${TESTDIR}/tests/testMsgInSetAction.o: tests/testMsgInSetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetAction.o tests/testMsgInSetAction.cpp


${TESTDIR}/tests/testrunnerMsgInSetAction.o: tests/testrunnerMsgInSetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetAction.o tests/testrunnerMsgInSetAction.cpp


${TESTDIR}/tests/testMsgInSetCondition.o: tests/testMsgInSetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetCondition.o tests/testMsgInSetCondition.cpp


${TESTDIR}/tests/testrunnerMsgInSetCondition.o: tests/testrunnerMsgInSetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetCondition.o tests/testrunnerMsgInSetCondition.cpp


${TESTDIR}/tests/testMsgInSetTimeZone.o: tests/testMsgInSetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetTimeZone.o tests/testMsgInSetTimeZone.cpp


${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o: tests/testrunnerMsgInSetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o tests/testrunnerMsgInSetTimeZone.cpp


${TESTDIR}/tests/testMsgInSignIn.o: tests/testMsgInSignIn.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSignIn.o tests/testMsgInSignIn.cpp


${TESTDIR}/tests/testrunnerMsgInSignIn.o: tests/testrunnerMsgInSignIn.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSignIn.o tests/testrunnerMsgInSignIn.cpp


${TESTDIR}/tests/testMsgInSignUp.o: tests/testMsgInSignUp.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSignUp.o tests/testMsgInSignUp.cpp


${TESTDIR}/tests/testrunnerMsgInSignUp.o: tests/testrunnerMsgInSignUp.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSignUp.o tests/testrunnerMsgInSignUp.cpp


${TESTDIR}/tests/testMsgInSwitch.o: tests/testMsgInSwitch.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSwitch.o tests/testMsgInSwitch.cpp


${TESTDIR}/tests/testrunnerMsgInSwitch.o: tests/testrunnerMsgInSwitch.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSwitch.o tests/testrunnerMsgInSwitch.cpp


${TESTDIR}/tests/testMsgInUpdate.o: tests/testMsgInUpdate.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdate.o tests/testMsgInUpdate.cpp


${TESTDIR}/tests/testrunnerMsgInUpdate.o: tests/testrunnerMsgInUpdate.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdate.o tests/testrunnerMsgInUpdate.cpp


${TESTDIR}/tests/testMsgInUpdateAction.o: tests/testMsgInUpdateAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateAction.o tests/testMsgInUpdateAction.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateAction.o: tests/testrunnerMsgInUpdateAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateAction.o tests/testrunnerMsgInUpdateAction.cpp


${TESTDIR}/tests/testMsgInUpdateCondition.o: tests/testMsgInUpdateCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateCondition.o tests/testMsgInUpdateCondition.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o: tests/testrunnerMsgInUpdateCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o tests/testrunnerMsgInUpdateCondition.cpp


${TESTDIR}/tests/testMsgInUpdateRooms.o: tests/testMsgInUpdateRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateRooms.o tests/testMsgInUpdateRooms.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o: tests/testrunnerMsgInUpdateRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o tests/testrunnerMsgInUpdateRooms.cpp


${OBJECTDIR}/_ext/1360890531/DAO_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAO.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO_nomain.o ../DAO/DAO.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters_nomain.o ../DAO/DAOAdapters.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices_nomain.o ../DAO/DAODevices.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices_nomain.o ../DAO/DAOMobileDevices.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAONotification_nomain.o ../DAO/DAONotification.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms_nomain.o ../DAO/DAORooms.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers_nomain.o ../DAO/DAOUsers.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters_nomain.o ../DAO/DAOUsersAdapters.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger_nomain.o ../ui_logger/Logger.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config_nomain.o Config.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector_nomain.o DBConnector.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory_nomain.o MsgInFactory.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInFactory.o ${OBJECTDIR}/MsgInFactory_nomain.o;\
	fi

${OBJECTDIR}/MsgRightsChecker_nomain.o: ${OBJECTDIR}/MsgRightsChecker.o MsgRightsChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgRightsChecker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgRightsChecker_nomain.o MsgRightsChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgRightsChecker.o ${OBJECTDIR}/MsgRightsChecker_nomain.o;\
	fi

${OBJECTDIR}/ServerException_nomain.o: ${OBJECTDIR}/ServerException.o ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ServerException.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException_nomain.o ServerException.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ServerException.o ${OBJECTDIR}/ServerException_nomain.o;\
	fi

${OBJECTDIR}/SocketClient_nomain.o: ${OBJECTDIR}/SocketClient.o SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/SocketClient.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient_nomain.o SocketClient.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer_nomain.o SocketServer.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication_nomain.o communication.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fTokenChecker_nomain.o fTokenChecker.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker_nomain.o gTokenChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/gTokenChecker.o ${OBJECTDIR}/gTokenChecker_nomain.o;\
	fi

${OBJECTDIR}/msgs/IMsgIn_nomain.o: ${OBJECTDIR}/msgs/IMsgIn.o msgs/IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/IMsgIn.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgIn_nomain.o msgs/IMsgIn.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginAndAdapterAccessRequired_nomain.o msgs/IMsgInLoginAndAdapterAccessRequired.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginRequired_nomain.o msgs/IMsgInLoginRequired.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/IMsgInLoginUnwanted_nomain.o msgs/IMsgInLoginUnwanted.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAdapterListen_nomain.o msgs/MsgInAdapterListen.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddAccount_nomain.o msgs/MsgInAddAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAddAccount.o ${OBJECTDIR}/msgs/MsgInAddAccount_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAddAdapter_nomain.o: ${OBJECTDIR}/msgs/MsgInAddAdapter.o msgs/MsgInAddAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAddAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddAdapter_nomain.o msgs/MsgInAddAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAddAdapter.o ${OBJECTDIR}/msgs/MsgInAddAdapter_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInAddRoom_nomain.o: ${OBJECTDIR}/msgs/MsgInAddRoom.o msgs/MsgInAddRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInAddRoom.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddRoom_nomain.o msgs/MsgInAddRoom.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAddView_nomain.o msgs/MsgInAddView.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o msgs/MsgInAlgorithmsRedirect.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect.o ${OBJECTDIR}/msgs/MsgInAlgorithmsRedirect_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInDelAdapter_nomain.o: ${OBJECTDIR}/msgs/MsgInDelAdapter.o msgs/MsgInDelAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInDelAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelAdapter_nomain.o msgs/MsgInDelAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInDelAdapter.o ${OBJECTDIR}/msgs/MsgInDelAdapter_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInDelConAccount_nomain.o: ${OBJECTDIR}/msgs/MsgInDelConAccount.o msgs/MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInDelConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelConAccount_nomain.o msgs/MsgInDelConAccount.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelDevice_nomain.o msgs/MsgInDelDevice.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelGCMID_nomain.o msgs/MsgInDelGCMID.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelRoom_nomain.o msgs/MsgInDelRoom.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInDelView_nomain.o msgs/MsgInDelView.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o msgs/MsgInGamificationRedirect.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGamificationRedirect.o ${OBJECTDIR}/msgs/MsgInGamificationRedirect_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetAdapters_nomain.o: ${OBJECTDIR}/msgs/MsgInGetAdapters.o msgs/MsgInGetAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetAdapters.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetAdapters_nomain.o msgs/MsgInGetAdapters.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetAdapters.o ${OBJECTDIR}/msgs/MsgInGetAdapters_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetAllDevices_nomain.o: ${OBJECTDIR}/msgs/MsgInGetAllDevices.o msgs/MsgInGetAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetAllDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetAllDevices_nomain.o msgs/MsgInGetAllDevices.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetConAccount_nomain.o msgs/MsgInGetConAccount.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetDevices_nomain.o msgs/MsgInGetDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetDevices.o ${OBJECTDIR}/msgs/MsgInGetDevices_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetNewDevices_nomain.o: ${OBJECTDIR}/msgs/MsgInGetNewDevices.o msgs/MsgInGetNewDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetNewDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNewDevices_nomain.o msgs/MsgInGetNewDevices.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetNotifications_nomain.o msgs/MsgInGetNotifications.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetRooms_nomain.o msgs/MsgInGetRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetRooms.o ${OBJECTDIR}/msgs/MsgInGetRooms_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetTimeZone_nomain.o: ${OBJECTDIR}/msgs/MsgInGetTimeZone.o msgs/MsgInGetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetTimeZone.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetTimeZone_nomain.o msgs/MsgInGetTimeZone.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetTimeZone.o ${OBJECTDIR}/msgs/MsgInGetTimeZone_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetUserID_nomain.o: ${OBJECTDIR}/msgs/MsgInGetUserID.o msgs/MsgInGetUserID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetUserID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetUserID_nomain.o msgs/MsgInGetUserID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetUserID.o ${OBJECTDIR}/msgs/MsgInGetUserID_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetUserInfo_nomain.o: ${OBJECTDIR}/msgs/MsgInGetUserInfo.o msgs/MsgInGetUserInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetUserInfo.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetUserInfo_nomain.o msgs/MsgInGetUserInfo.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInGetUserInfo.o ${OBJECTDIR}/msgs/MsgInGetUserInfo_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInGetViews_nomain.o: ${OBJECTDIR}/msgs/MsgInGetViews.o msgs/MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInGetViews.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInGetViews_nomain.o msgs/MsgInGetViews.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInNotificationRead_nomain.o msgs/MsgInNotificationRead.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetConAccount_nomain.o msgs/MsgInSetConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSetConAccount.o ${OBJECTDIR}/msgs/MsgInSetConAccount_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSetGCMID_nomain.o: ${OBJECTDIR}/msgs/MsgInSetGCMID.o msgs/MsgInSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSetGCMID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetGCMID_nomain.o msgs/MsgInSetGCMID.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetRooms_nomain.o msgs/MsgInSetRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSetRooms.o ${OBJECTDIR}/msgs/MsgInSetRooms_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSetTimeZone_nomain.o: ${OBJECTDIR}/msgs/MsgInSetTimeZone.o msgs/MsgInSetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSetTimeZone.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSetTimeZone_nomain.o msgs/MsgInSetTimeZone.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSetTimeZone.o ${OBJECTDIR}/msgs/MsgInSetTimeZone_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSignMe_nomain.o: ${OBJECTDIR}/msgs/MsgInSignMe.o msgs/MsgInSignMe.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSignMe.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSignMe_nomain.o msgs/MsgInSignMe.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSignMe.o ${OBJECTDIR}/msgs/MsgInSignMe_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSignUp_nomain.o: ${OBJECTDIR}/msgs/MsgInSignUp.o msgs/MsgInSignUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSignUp.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSignUp_nomain.o msgs/MsgInSignUp.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInSignUp.o ${OBJECTDIR}/msgs/MsgInSignUp_nomain.o;\
	fi

${OBJECTDIR}/msgs/MsgInSwitch_nomain.o: ${OBJECTDIR}/msgs/MsgInSwitch.o msgs/MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}/msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgs/MsgInSwitch.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInSwitch_nomain.o msgs/MsgInSwitch.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUnknown_nomain.o msgs/MsgInUnknown.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgs/MsgInUpdateView_nomain.o msgs/MsgInUpdateView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgs/MsgInUpdateView.o ${OBJECTDIR}/msgs/MsgInUpdateView_nomain.o;\
	fi

${OBJECTDIR}/pugixml_nomain.o: ${OBJECTDIR}/pugixml.o pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/pugixml.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml_nomain.o pugixml.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer_nomain.o save_custom_writer.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet_nomain.o sslServlet.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/sslServlet.o ${OBJECTDIR}/sslServlet_nomain.o;\
	fi

${OBJECTDIR}/testHelper_nomain.o: ${OBJECTDIR}/testHelper.o testHelper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/testHelper.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/testHelper_nomain.o testHelper.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/testHelper.o ${OBJECTDIR}/testHelper_nomain.o;\
	fi

${OBJECTDIR}/ui_server_nomain.o: ${OBJECTDIR}/ui_server.o ui_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ui_server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/soci -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server_nomain.o ui_server.cpp;\
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
