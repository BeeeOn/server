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
	${OBJECTDIR}/_ext/1360890531/DAORooms.o \
	${OBJECTDIR}/_ext/1360890531/DAOUsers.o \
	${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o \
	${OBJECTDIR}/_ext/1154895860/Logger.o \
	${OBJECTDIR}/ComTable.o \
	${OBJECTDIR}/Config.o \
	${OBJECTDIR}/DBConnector.o \
	${OBJECTDIR}/IMsgIn.o \
	${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o \
	${OBJECTDIR}/IMsgInLoginRequired.o \
	${OBJECTDIR}/IMsgInLoginUnwanted.o \
	${OBJECTDIR}/MsgInAdapterListen.o \
	${OBJECTDIR}/MsgInAddAccount.o \
	${OBJECTDIR}/MsgInAddAdapter.o \
	${OBJECTDIR}/MsgInAddRoom.o \
	${OBJECTDIR}/MsgInAddView.o \
	${OBJECTDIR}/MsgInAlgorithmsRedirect.o \
	${OBJECTDIR}/MsgInDelConAccount.o \
	${OBJECTDIR}/MsgInDelDevice.o \
	${OBJECTDIR}/MsgInDelGCMID.o \
	${OBJECTDIR}/MsgInDelRoom.o \
	${OBJECTDIR}/MsgInDelView.o \
	${OBJECTDIR}/MsgInFactory.o \
	${OBJECTDIR}/MsgInGetAdapters.o \
	${OBJECTDIR}/MsgInGetAllDevices.o \
	${OBJECTDIR}/MsgInGetConAccount.o \
	${OBJECTDIR}/MsgInGetDevices.o \
	${OBJECTDIR}/MsgInGetLog.o \
	${OBJECTDIR}/MsgInGetNewDevices.o \
	${OBJECTDIR}/MsgInGetRooms.o \
	${OBJECTDIR}/MsgInGetTimeZone.o \
	${OBJECTDIR}/MsgInGetUserID.o \
	${OBJECTDIR}/MsgInGetUserInfo.o \
	${OBJECTDIR}/MsgInGetViews.o \
	${OBJECTDIR}/MsgInReinitAdapter.o \
	${OBJECTDIR}/MsgInSetConAccount.o \
	${OBJECTDIR}/MsgInSetDevices.o \
	${OBJECTDIR}/MsgInSetGCMID.o \
	${OBJECTDIR}/MsgInSetRooms.o \
	${OBJECTDIR}/MsgInSetTimeZone.o \
	${OBJECTDIR}/MsgInSignMe.o \
	${OBJECTDIR}/MsgInSwitch.o \
	${OBJECTDIR}/MsgInUnknown.o \
	${OBJECTDIR}/MsgInUpdateView.o \
	${OBJECTDIR}/MsgRightsChecker.o \
	${OBJECTDIR}/Msgs/MsgInDelAdapter.o \
	${OBJECTDIR}/Msgs/MsgInSignUp.o \
	${OBJECTDIR}/ServerException.o \
	${OBJECTDIR}/SocketClient.o \
	${OBJECTDIR}/SocketServer.o \
	${OBJECTDIR}/communication.o \
	${OBJECTDIR}/gTokenChecker.o \
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
LDLIBSOPTIONS=-L/usr/local/lib -L/usr/local/lib64 -L/opt/centos/devtoolset-1.1/root/usr/lib/gcc/x86_64-redhat-linux/4.7.2 -L/usr/pgsql-9.2/lib -Wl,-rpath,/usr/local/lib -Wl,-rpath,/usr/local/lib64 -lsoci_core -lsoci_empty -lsoci_postgresql -lpq -ljansson `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ./ui_server

./ui_server: ${OBJECTFILES}
	${MKDIR} -p .
	g++ -o ./ui_server ${OBJECTFILES} ${LDLIBSOPTIONS} -lpq -lsoci_core -lsoci_empty -lsoci_postgresql -lcppunit -lssl -lcrypto -ljansson -lstdc++

${OBJECTDIR}/_ext/1360890531/DAO.o: ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp

${OBJECTDIR}/_ext/1360890531/DAOAdapters.o: ../DAO/DAOAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters.o ../DAO/DAOAdapters.cpp

${OBJECTDIR}/_ext/1360890531/DAODevices.o: ../DAO/DAODevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices.o ../DAO/DAODevices.cpp

${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o: ../DAO/DAOMobileDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o ../DAO/DAOMobileDevices.cpp

${OBJECTDIR}/_ext/1360890531/DAORooms.o: ../DAO/DAORooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms.o ../DAO/DAORooms.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsers.o: ../DAO/DAOUsers.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers.o ../DAO/DAOUsers.cpp

${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o: ../DAO/DAOUsersAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters.o ../DAO/DAOUsersAdapters.cpp

${OBJECTDIR}/_ext/1154895860/Logger.o: ../ui_logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1154895860
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger.o ../ui_logger/Logger.cpp

${OBJECTDIR}/ComTable.o: ComTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComTable.o ComTable.cpp

${OBJECTDIR}/Config.o: Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config.o Config.cpp

${OBJECTDIR}/DBConnector.o: DBConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector.o DBConnector.cpp

${OBJECTDIR}/IMsgIn.o: IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgIn.o IMsgIn.cpp

${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o: IMsgInLoginAndAdapterAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o IMsgInLoginAndAdapterAccessRequired.cpp

${OBJECTDIR}/IMsgInLoginRequired.o: IMsgInLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginRequired.o IMsgInLoginRequired.cpp

${OBJECTDIR}/IMsgInLoginUnwanted.o: IMsgInLoginUnwanted.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginUnwanted.o IMsgInLoginUnwanted.cpp

${OBJECTDIR}/MsgInAdapterListen.o: MsgInAdapterListen.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAdapterListen.o MsgInAdapterListen.cpp

${OBJECTDIR}/MsgInAddAccount.o: MsgInAddAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAccount.o MsgInAddAccount.cpp

${OBJECTDIR}/MsgInAddAdapter.o: MsgInAddAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAdapter.o MsgInAddAdapter.cpp

${OBJECTDIR}/MsgInAddRoom.o: MsgInAddRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddRoom.o MsgInAddRoom.cpp

${OBJECTDIR}/MsgInAddView.o: MsgInAddView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddView.o MsgInAddView.cpp

${OBJECTDIR}/MsgInAlgorithmsRedirect.o: MsgInAlgorithmsRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAlgorithmsRedirect.o MsgInAlgorithmsRedirect.cpp

${OBJECTDIR}/MsgInDelConAccount.o: MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelConAccount.o MsgInDelConAccount.cpp

${OBJECTDIR}/MsgInDelDevice.o: MsgInDelDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelDevice.o MsgInDelDevice.cpp

${OBJECTDIR}/MsgInDelGCMID.o: MsgInDelGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelGCMID.o MsgInDelGCMID.cpp

${OBJECTDIR}/MsgInDelRoom.o: MsgInDelRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelRoom.o MsgInDelRoom.cpp

${OBJECTDIR}/MsgInDelView.o: MsgInDelView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelView.o MsgInDelView.cpp

${OBJECTDIR}/MsgInFactory.o: MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp

${OBJECTDIR}/MsgInGetAdapters.o: MsgInGetAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAdapters.o MsgInGetAdapters.cpp

${OBJECTDIR}/MsgInGetAllDevices.o: MsgInGetAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAllDevices.o MsgInGetAllDevices.cpp

${OBJECTDIR}/MsgInGetConAccount.o: MsgInGetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetConAccount.o MsgInGetConAccount.cpp

${OBJECTDIR}/MsgInGetDevices.o: MsgInGetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetDevices.o MsgInGetDevices.cpp

${OBJECTDIR}/MsgInGetLog.o: MsgInGetLog.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetLog.o MsgInGetLog.cpp

${OBJECTDIR}/MsgInGetNewDevices.o: MsgInGetNewDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetNewDevices.o MsgInGetNewDevices.cpp

${OBJECTDIR}/MsgInGetRooms.o: MsgInGetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetRooms.o MsgInGetRooms.cpp

${OBJECTDIR}/MsgInGetTimeZone.o: MsgInGetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetTimeZone.o MsgInGetTimeZone.cpp

${OBJECTDIR}/MsgInGetUserID.o: MsgInGetUserID.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetUserID.o MsgInGetUserID.cpp

${OBJECTDIR}/MsgInGetUserInfo.o: MsgInGetUserInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetUserInfo.o MsgInGetUserInfo.cpp

${OBJECTDIR}/MsgInGetViews.o: MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetViews.o MsgInGetViews.cpp

${OBJECTDIR}/MsgInReinitAdapter.o: MsgInReinitAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInReinitAdapter.o MsgInReinitAdapter.cpp

${OBJECTDIR}/MsgInSetConAccount.o: MsgInSetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetConAccount.o MsgInSetConAccount.cpp

${OBJECTDIR}/MsgInSetDevices.o: MsgInSetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetDevices.o MsgInSetDevices.cpp

${OBJECTDIR}/MsgInSetGCMID.o: MsgInSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetGCMID.o MsgInSetGCMID.cpp

${OBJECTDIR}/MsgInSetRooms.o: MsgInSetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetRooms.o MsgInSetRooms.cpp

${OBJECTDIR}/MsgInSetTimeZone.o: MsgInSetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetTimeZone.o MsgInSetTimeZone.cpp

${OBJECTDIR}/MsgInSignMe.o: MsgInSignMe.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSignMe.o MsgInSignMe.cpp

${OBJECTDIR}/MsgInSwitch.o: MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSwitch.o MsgInSwitch.cpp

${OBJECTDIR}/MsgInUnknown.o: MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUnknown.o MsgInUnknown.cpp

${OBJECTDIR}/MsgInUpdateView.o: MsgInUpdateView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateView.o MsgInUpdateView.cpp

${OBJECTDIR}/MsgRightsChecker.o: MsgRightsChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgRightsChecker.o MsgRightsChecker.cpp

${OBJECTDIR}/Msgs/MsgInDelAdapter.o: Msgs/MsgInDelAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/Msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Msgs/MsgInDelAdapter.o Msgs/MsgInDelAdapter.cpp

${OBJECTDIR}/Msgs/MsgInSignUp.o: Msgs/MsgInSignUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/Msgs
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Msgs/MsgInSignUp.o Msgs/MsgInSignUp.cpp

${OBJECTDIR}/ServerException.o: ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException.o ServerException.cpp

${OBJECTDIR}/SocketClient.o: SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient.o SocketClient.cpp

${OBJECTDIR}/SocketServer.o: SocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer.o SocketServer.cpp

${OBJECTDIR}/communication.o: communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication.o communication.cpp

${OBJECTDIR}/gTokenChecker.o: gTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker.o gTokenChecker.cpp

${OBJECTDIR}/pugixml.o: pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml.o pugixml.cpp

${OBJECTDIR}/save_custom_writer.o: save_custom_writer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer.o save_custom_writer.cpp

${OBJECTDIR}/sslServlet.o: sslServlet.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet.o sslServlet.cpp

${OBJECTDIR}/testHelper.o: testHelper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/testHelper.o testHelper.cpp

${OBJECTDIR}/ui_server.o: ui_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server.o ui_server.cpp

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
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestclass.o tests/newtestclass.cpp


${TESTDIR}/tests/newtestrunner.o: tests/newtestrunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestrunner.o tests/newtestrunner.cpp


${TESTDIR}/tests/testChangeConAccount.o: tests/testChangeConAccount.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testChangeConAccount.o tests/testChangeConAccount.cpp


${TESTDIR}/tests/testrunnerChangeConAccount.o: tests/testrunnerChangeConAccount.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerChangeConAccount.o tests/testrunnerChangeConAccount.cpp


${TESTDIR}/tests/testGlobal.o: tests/testGlobal.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testGlobal.o tests/testGlobal.cpp


${TESTDIR}/tests/testrunnerGlobal.o: tests/testrunnerGlobal.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerGlobal.o tests/testrunnerGlobal.cpp


${TESTDIR}/tests/testMsgInAddAdapter.o: tests/testMsgInAddAdapter.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddAdapter.o tests/testMsgInAddAdapter.cpp


${TESTDIR}/tests/testrunnerMsgInAddAdapter.o: tests/testrunnerMsgInAddAdapter.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddAdapter.o tests/testrunnerMsgInAddAdapter.cpp


${TESTDIR}/tests/testMsgInAddConAcc.o: tests/testMsgInAddConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddConAcc.o tests/testMsgInAddConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInAddConAcc.o: tests/testrunnerMsgInAddConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddConAcc.o tests/testrunnerMsgInAddConAcc.cpp


${TESTDIR}/tests/testMsgInAddRooms.o: tests/testMsgInAddRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddRooms.o tests/testMsgInAddRooms.cpp


${TESTDIR}/tests/testrunnerMsgInAddRooms.o: tests/testrunnerMsgInAddRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddRooms.o tests/testrunnerMsgInAddRooms.cpp


${TESTDIR}/tests/testMsgInDelAction.o: tests/testMsgInDelAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelAction.o tests/testMsgInDelAction.cpp


${TESTDIR}/tests/testrunnerMsgInDelAction.o: tests/testrunnerMsgInDelAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelAction.o tests/testrunnerMsgInDelAction.cpp


${TESTDIR}/tests/testMsgInDelConAcc.o: tests/testMsgInDelConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelConAcc.o tests/testMsgInDelConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInDelConAcc.o: tests/testrunnerMsgInDelConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelConAcc.o tests/testrunnerMsgInDelConAcc.cpp


${TESTDIR}/tests/testMsgInDelDevice.o: tests/testMsgInDelDevice.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelDevice.o tests/testMsgInDelDevice.cpp


${TESTDIR}/tests/testrunnerMsgInDelDevice.o: tests/testrunnerMsgInDelDevice.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelDevice.o tests/testrunnerMsgInDelDevice.cpp


${TESTDIR}/tests/testMsgInDeleteCondition.o: tests/testMsgInDeleteCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDeleteCondition.o tests/testMsgInDeleteCondition.cpp


${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o: tests/testrunnerMsgInDeleteCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o tests/testrunnerMsgInDeleteCondition.cpp


${TESTDIR}/tests/testMsgInDelRoom.o: tests/testMsgInDelRoom.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelRoom.o tests/testMsgInDelRoom.cpp


${TESTDIR}/tests/testrunnerMsgInDelRoom.o: tests/testrunnerMsgInDelRoom.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelRoom.o tests/testrunnerMsgInDelRoom.cpp


${TESTDIR}/tests/testMsgInPartial.o: tests/testMsgInPartial.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInPartial.o tests/testMsgInPartial.cpp


${TESTDIR}/tests/testrunnerMsgInPartial.o: tests/testrunnerMsgInPartial.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInPartial.o tests/testrunnerMsgInPartial.cpp


${TESTDIR}/tests/testMsgInGetAction.o: tests/testMsgInGetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetAction.o tests/testMsgInGetAction.cpp


${TESTDIR}/tests/testrunnerMsgInGetAction.o: tests/testrunnerMsgInGetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetAction.o tests/testrunnerMsgInGetAction.cpp


${TESTDIR}/tests/testMsgInGetActions.o: tests/testMsgInGetActions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetActions.o tests/testMsgInGetActions.cpp


${TESTDIR}/tests/testrunnerMsgInGetActions.o: tests/testrunnerMsgInGetActions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetActions.o tests/testrunnerMsgInGetActions.cpp


${TESTDIR}/tests/testMsgInGetAdapters.o: tests/testMsgInGetAdapters.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetAdapters.o tests/testMsgInGetAdapters.cpp


${TESTDIR}/tests/testrunnerMsgInGetAdapters.o: tests/testrunnerMsgInGetAdapters.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetAdapters.o tests/testrunnerMsgInGetAdapters.cpp


${TESTDIR}/tests/testMsgInGetConAcc.o: tests/testMsgInGetConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetConAcc.o tests/testMsgInGetConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInGetConAcc.o: tests/testrunnerMsgInGetConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetConAcc.o tests/testrunnerMsgInGetConAcc.cpp


${TESTDIR}/tests/testMsgInGetConditions.o: tests/testMsgInGetConditions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetConditions.o tests/testMsgInGetConditions.cpp


${TESTDIR}/tests/testrunnerMsgInGetConditions.o: tests/testrunnerMsgInGetConditions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetConditions.o tests/testrunnerMsgInGetConditions.cpp


${TESTDIR}/tests/testMsgInGetDevs.o: tests/testMsgInGetDevs.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetDevs.o tests/testMsgInGetDevs.cpp


${TESTDIR}/tests/testrunnerMsgInGetDevs.o: tests/testrunnerMsgInGetDevs.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetDevs.o tests/testrunnerMsgInGetDevs.cpp


${TESTDIR}/tests/testMsgInGetRooms.o: tests/testMsgInGetRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetRooms.o tests/testMsgInGetRooms.cpp


${TESTDIR}/tests/testrunnerMsgInGetRooms.o: tests/testrunnerMsgInGetRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetRooms.o tests/testrunnerMsgInGetRooms.cpp


${TESTDIR}/tests/testMsgInGetTimeZone.o: tests/testMsgInGetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetTimeZone.o tests/testMsgInGetTimeZone.cpp


${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o: tests/testrunnerMsgInGetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o tests/testrunnerMsgInGetTimeZone.cpp


${TESTDIR}/tests/testMsgInLogName.o: tests/testMsgInLogName.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInLogName.o tests/testMsgInLogName.cpp


${TESTDIR}/tests/testrunnerMsgInLogName.o: tests/testrunnerMsgInLogName.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInLogName.o tests/testrunnerMsgInLogName.cpp


${TESTDIR}/tests/testMsgInSetAction.o: tests/testMsgInSetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetAction.o tests/testMsgInSetAction.cpp


${TESTDIR}/tests/testrunnerMsgInSetAction.o: tests/testrunnerMsgInSetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetAction.o tests/testrunnerMsgInSetAction.cpp


${TESTDIR}/tests/testMsgInSetCondition.o: tests/testMsgInSetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetCondition.o tests/testMsgInSetCondition.cpp


${TESTDIR}/tests/testrunnerMsgInSetCondition.o: tests/testrunnerMsgInSetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetCondition.o tests/testrunnerMsgInSetCondition.cpp


${TESTDIR}/tests/testMsgInSetTimeZone.o: tests/testMsgInSetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetTimeZone.o tests/testMsgInSetTimeZone.cpp


${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o: tests/testrunnerMsgInSetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o tests/testrunnerMsgInSetTimeZone.cpp


${TESTDIR}/tests/testMsgInSignIn.o: tests/testMsgInSignIn.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSignIn.o tests/testMsgInSignIn.cpp


${TESTDIR}/tests/testrunnerMsgInSignIn.o: tests/testrunnerMsgInSignIn.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSignIn.o tests/testrunnerMsgInSignIn.cpp


${TESTDIR}/tests/testMsgInSignUp.o: tests/testMsgInSignUp.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSignUp.o tests/testMsgInSignUp.cpp


${TESTDIR}/tests/testrunnerMsgInSignUp.o: tests/testrunnerMsgInSignUp.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSignUp.o tests/testrunnerMsgInSignUp.cpp


${TESTDIR}/tests/testMsgInSwitch.o: tests/testMsgInSwitch.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSwitch.o tests/testMsgInSwitch.cpp


${TESTDIR}/tests/testrunnerMsgInSwitch.o: tests/testrunnerMsgInSwitch.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSwitch.o tests/testrunnerMsgInSwitch.cpp


${TESTDIR}/tests/testMsgInUpdate.o: tests/testMsgInUpdate.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdate.o tests/testMsgInUpdate.cpp


${TESTDIR}/tests/testrunnerMsgInUpdate.o: tests/testrunnerMsgInUpdate.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdate.o tests/testrunnerMsgInUpdate.cpp


${TESTDIR}/tests/testMsgInUpdateAction.o: tests/testMsgInUpdateAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateAction.o tests/testMsgInUpdateAction.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateAction.o: tests/testrunnerMsgInUpdateAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateAction.o tests/testrunnerMsgInUpdateAction.cpp


${TESTDIR}/tests/testMsgInUpdateCondition.o: tests/testMsgInUpdateCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateCondition.o tests/testMsgInUpdateCondition.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o: tests/testrunnerMsgInUpdateCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o tests/testrunnerMsgInUpdateCondition.cpp


${TESTDIR}/tests/testMsgInUpdateRooms.o: tests/testMsgInUpdateRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateRooms.o tests/testMsgInUpdateRooms.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o: tests/testrunnerMsgInUpdateRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -I. `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o tests/testrunnerMsgInUpdateRooms.cpp


${OBJECTDIR}/_ext/1360890531/DAO_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAO.o ../DAO/DAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAO.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAO_nomain.o ../DAO/DAO.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOAdapters_nomain.o ../DAO/DAOAdapters.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAODevices_nomain.o ../DAO/DAODevices.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices_nomain.o ../DAO/DAOMobileDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices.o ${OBJECTDIR}/_ext/1360890531/DAOMobileDevices_nomain.o;\
	fi

${OBJECTDIR}/_ext/1360890531/DAORooms_nomain.o: ${OBJECTDIR}/_ext/1360890531/DAORooms.o ../DAO/DAORooms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360890531
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1360890531/DAORooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAORooms_nomain.o ../DAO/DAORooms.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsers_nomain.o ../DAO/DAOUsers.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360890531/DAOUsersAdapters_nomain.o ../DAO/DAOUsersAdapters.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1154895860/Logger_nomain.o ../ui_logger/Logger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1154895860/Logger.o ${OBJECTDIR}/_ext/1154895860/Logger_nomain.o;\
	fi

${OBJECTDIR}/ComTable_nomain.o: ${OBJECTDIR}/ComTable.o ComTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ComTable.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComTable_nomain.o ComTable.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ComTable.o ${OBJECTDIR}/ComTable_nomain.o;\
	fi

${OBJECTDIR}/Config_nomain.o: ${OBJECTDIR}/Config.o Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Config.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config_nomain.o Config.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector_nomain.o DBConnector.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/DBConnector.o ${OBJECTDIR}/DBConnector_nomain.o;\
	fi

${OBJECTDIR}/IMsgIn_nomain.o: ${OBJECTDIR}/IMsgIn.o IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/IMsgIn.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgIn_nomain.o IMsgIn.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/IMsgIn.o ${OBJECTDIR}/IMsgIn_nomain.o;\
	fi

${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired_nomain.o: ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o IMsgInLoginAndAdapterAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired_nomain.o IMsgInLoginAndAdapterAccessRequired.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired_nomain.o;\
	fi

${OBJECTDIR}/IMsgInLoginRequired_nomain.o: ${OBJECTDIR}/IMsgInLoginRequired.o IMsgInLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/IMsgInLoginRequired.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginRequired_nomain.o IMsgInLoginRequired.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/IMsgInLoginRequired.o ${OBJECTDIR}/IMsgInLoginRequired_nomain.o;\
	fi

${OBJECTDIR}/IMsgInLoginUnwanted_nomain.o: ${OBJECTDIR}/IMsgInLoginUnwanted.o IMsgInLoginUnwanted.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/IMsgInLoginUnwanted.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginUnwanted_nomain.o IMsgInLoginUnwanted.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/IMsgInLoginUnwanted.o ${OBJECTDIR}/IMsgInLoginUnwanted_nomain.o;\
	fi

${OBJECTDIR}/MsgInAdapterListen_nomain.o: ${OBJECTDIR}/MsgInAdapterListen.o MsgInAdapterListen.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInAdapterListen.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAdapterListen_nomain.o MsgInAdapterListen.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInAdapterListen.o ${OBJECTDIR}/MsgInAdapterListen_nomain.o;\
	fi

${OBJECTDIR}/MsgInAddAccount_nomain.o: ${OBJECTDIR}/MsgInAddAccount.o MsgInAddAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInAddAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAccount_nomain.o MsgInAddAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInAddAccount.o ${OBJECTDIR}/MsgInAddAccount_nomain.o;\
	fi

${OBJECTDIR}/MsgInAddAdapter_nomain.o: ${OBJECTDIR}/MsgInAddAdapter.o MsgInAddAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInAddAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAdapter_nomain.o MsgInAddAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInAddAdapter.o ${OBJECTDIR}/MsgInAddAdapter_nomain.o;\
	fi

${OBJECTDIR}/MsgInAddRoom_nomain.o: ${OBJECTDIR}/MsgInAddRoom.o MsgInAddRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInAddRoom.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddRoom_nomain.o MsgInAddRoom.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInAddRoom.o ${OBJECTDIR}/MsgInAddRoom_nomain.o;\
	fi

${OBJECTDIR}/MsgInAddView_nomain.o: ${OBJECTDIR}/MsgInAddView.o MsgInAddView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInAddView.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddView_nomain.o MsgInAddView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInAddView.o ${OBJECTDIR}/MsgInAddView_nomain.o;\
	fi

${OBJECTDIR}/MsgInAlgorithmsRedirect_nomain.o: ${OBJECTDIR}/MsgInAlgorithmsRedirect.o MsgInAlgorithmsRedirect.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInAlgorithmsRedirect.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAlgorithmsRedirect_nomain.o MsgInAlgorithmsRedirect.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInAlgorithmsRedirect.o ${OBJECTDIR}/MsgInAlgorithmsRedirect_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelConAccount_nomain.o: ${OBJECTDIR}/MsgInDelConAccount.o MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelConAccount_nomain.o MsgInDelConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelConAccount.o ${OBJECTDIR}/MsgInDelConAccount_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelDevice_nomain.o: ${OBJECTDIR}/MsgInDelDevice.o MsgInDelDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelDevice.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelDevice_nomain.o MsgInDelDevice.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelDevice.o ${OBJECTDIR}/MsgInDelDevice_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelGCMID_nomain.o: ${OBJECTDIR}/MsgInDelGCMID.o MsgInDelGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelGCMID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelGCMID_nomain.o MsgInDelGCMID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelGCMID.o ${OBJECTDIR}/MsgInDelGCMID_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelRoom_nomain.o: ${OBJECTDIR}/MsgInDelRoom.o MsgInDelRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelRoom.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelRoom_nomain.o MsgInDelRoom.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelRoom.o ${OBJECTDIR}/MsgInDelRoom_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelView_nomain.o: ${OBJECTDIR}/MsgInDelView.o MsgInDelView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelView.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelView_nomain.o MsgInDelView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelView.o ${OBJECTDIR}/MsgInDelView_nomain.o;\
	fi

${OBJECTDIR}/MsgInFactory_nomain.o: ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInFactory.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory_nomain.o MsgInFactory.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInFactory.o ${OBJECTDIR}/MsgInFactory_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetAdapters_nomain.o: ${OBJECTDIR}/MsgInGetAdapters.o MsgInGetAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetAdapters.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAdapters_nomain.o MsgInGetAdapters.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetAdapters.o ${OBJECTDIR}/MsgInGetAdapters_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetAllDevices_nomain.o: ${OBJECTDIR}/MsgInGetAllDevices.o MsgInGetAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetAllDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAllDevices_nomain.o MsgInGetAllDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetAllDevices.o ${OBJECTDIR}/MsgInGetAllDevices_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetConAccount_nomain.o: ${OBJECTDIR}/MsgInGetConAccount.o MsgInGetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetConAccount_nomain.o MsgInGetConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetConAccount.o ${OBJECTDIR}/MsgInGetConAccount_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetDevices_nomain.o: ${OBJECTDIR}/MsgInGetDevices.o MsgInGetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetDevices_nomain.o MsgInGetDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetDevices.o ${OBJECTDIR}/MsgInGetDevices_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetLog_nomain.o: ${OBJECTDIR}/MsgInGetLog.o MsgInGetLog.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetLog.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetLog_nomain.o MsgInGetLog.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetLog.o ${OBJECTDIR}/MsgInGetLog_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetNewDevices_nomain.o: ${OBJECTDIR}/MsgInGetNewDevices.o MsgInGetNewDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetNewDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetNewDevices_nomain.o MsgInGetNewDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetNewDevices.o ${OBJECTDIR}/MsgInGetNewDevices_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetRooms_nomain.o: ${OBJECTDIR}/MsgInGetRooms.o MsgInGetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetRooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetRooms_nomain.o MsgInGetRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetRooms.o ${OBJECTDIR}/MsgInGetRooms_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetTimeZone_nomain.o: ${OBJECTDIR}/MsgInGetTimeZone.o MsgInGetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetTimeZone.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetTimeZone_nomain.o MsgInGetTimeZone.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetTimeZone.o ${OBJECTDIR}/MsgInGetTimeZone_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetUserID_nomain.o: ${OBJECTDIR}/MsgInGetUserID.o MsgInGetUserID.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetUserID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetUserID_nomain.o MsgInGetUserID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetUserID.o ${OBJECTDIR}/MsgInGetUserID_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetUserInfo_nomain.o: ${OBJECTDIR}/MsgInGetUserInfo.o MsgInGetUserInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetUserInfo.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetUserInfo_nomain.o MsgInGetUserInfo.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetUserInfo.o ${OBJECTDIR}/MsgInGetUserInfo_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetViews_nomain.o: ${OBJECTDIR}/MsgInGetViews.o MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetViews.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetViews_nomain.o MsgInGetViews.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetViews.o ${OBJECTDIR}/MsgInGetViews_nomain.o;\
	fi

${OBJECTDIR}/MsgInReinitAdapter_nomain.o: ${OBJECTDIR}/MsgInReinitAdapter.o MsgInReinitAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInReinitAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInReinitAdapter_nomain.o MsgInReinitAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInReinitAdapter.o ${OBJECTDIR}/MsgInReinitAdapter_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetConAccount_nomain.o: ${OBJECTDIR}/MsgInSetConAccount.o MsgInSetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetConAccount_nomain.o MsgInSetConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetConAccount.o ${OBJECTDIR}/MsgInSetConAccount_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetDevices_nomain.o: ${OBJECTDIR}/MsgInSetDevices.o MsgInSetDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetDevices_nomain.o MsgInSetDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetDevices.o ${OBJECTDIR}/MsgInSetDevices_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetGCMID_nomain.o: ${OBJECTDIR}/MsgInSetGCMID.o MsgInSetGCMID.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetGCMID.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetGCMID_nomain.o MsgInSetGCMID.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetGCMID.o ${OBJECTDIR}/MsgInSetGCMID_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetRooms_nomain.o: ${OBJECTDIR}/MsgInSetRooms.o MsgInSetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetRooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetRooms_nomain.o MsgInSetRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetRooms.o ${OBJECTDIR}/MsgInSetRooms_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetTimeZone_nomain.o: ${OBJECTDIR}/MsgInSetTimeZone.o MsgInSetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetTimeZone.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetTimeZone_nomain.o MsgInSetTimeZone.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetTimeZone.o ${OBJECTDIR}/MsgInSetTimeZone_nomain.o;\
	fi

${OBJECTDIR}/MsgInSignMe_nomain.o: ${OBJECTDIR}/MsgInSignMe.o MsgInSignMe.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSignMe.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSignMe_nomain.o MsgInSignMe.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSignMe.o ${OBJECTDIR}/MsgInSignMe_nomain.o;\
	fi

${OBJECTDIR}/MsgInSwitch_nomain.o: ${OBJECTDIR}/MsgInSwitch.o MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSwitch.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSwitch_nomain.o MsgInSwitch.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSwitch.o ${OBJECTDIR}/MsgInSwitch_nomain.o;\
	fi

${OBJECTDIR}/MsgInUnknown_nomain.o: ${OBJECTDIR}/MsgInUnknown.o MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInUnknown.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUnknown_nomain.o MsgInUnknown.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInUnknown.o ${OBJECTDIR}/MsgInUnknown_nomain.o;\
	fi

${OBJECTDIR}/MsgInUpdateView_nomain.o: ${OBJECTDIR}/MsgInUpdateView.o MsgInUpdateView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInUpdateView.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateView_nomain.o MsgInUpdateView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInUpdateView.o ${OBJECTDIR}/MsgInUpdateView_nomain.o;\
	fi

${OBJECTDIR}/MsgRightsChecker_nomain.o: ${OBJECTDIR}/MsgRightsChecker.o MsgRightsChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgRightsChecker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgRightsChecker_nomain.o MsgRightsChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgRightsChecker.o ${OBJECTDIR}/MsgRightsChecker_nomain.o;\
	fi

${OBJECTDIR}/Msgs/MsgInDelAdapter_nomain.o: ${OBJECTDIR}/Msgs/MsgInDelAdapter.o Msgs/MsgInDelAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}/Msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Msgs/MsgInDelAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Msgs/MsgInDelAdapter_nomain.o Msgs/MsgInDelAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Msgs/MsgInDelAdapter.o ${OBJECTDIR}/Msgs/MsgInDelAdapter_nomain.o;\
	fi

${OBJECTDIR}/Msgs/MsgInSignUp_nomain.o: ${OBJECTDIR}/Msgs/MsgInSignUp.o Msgs/MsgInSignUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/Msgs
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Msgs/MsgInSignUp.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Msgs/MsgInSignUp_nomain.o Msgs/MsgInSignUp.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Msgs/MsgInSignUp.o ${OBJECTDIR}/Msgs/MsgInSignUp_nomain.o;\
	fi

${OBJECTDIR}/ServerException_nomain.o: ${OBJECTDIR}/ServerException.o ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ServerException.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException_nomain.o ServerException.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient_nomain.o SocketClient.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketServer_nomain.o SocketServer.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication_nomain.o communication.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/communication.o ${OBJECTDIR}/communication_nomain.o;\
	fi

${OBJECTDIR}/gTokenChecker_nomain.o: ${OBJECTDIR}/gTokenChecker.o gTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/gTokenChecker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker_nomain.o gTokenChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/gTokenChecker.o ${OBJECTDIR}/gTokenChecker_nomain.o;\
	fi

${OBJECTDIR}/pugixml_nomain.o: ${OBJECTDIR}/pugixml.o pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/pugixml.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml_nomain.o pugixml.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/save_custom_writer_nomain.o save_custom_writer.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sslServlet_nomain.o sslServlet.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/testHelper_nomain.o testHelper.cpp;\
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
	    $(COMPILE.cc) -g -Wall -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I/usr/include -I/usr/local/include/soci -Iusr/include/openssl -I../Server -I. -I/usr/pgsql-9.2/include -I/usr/pgsql-9.2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ui_server_nomain.o ui_server.cpp;\
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
