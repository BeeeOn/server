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
	${OBJECTDIR}/ComTable.o \
	${OBJECTDIR}/Config.o \
	${OBJECTDIR}/DBConnector.o \
	${OBJECTDIR}/IMsgIn.o \
	${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o \
	${OBJECTDIR}/IMsgInLoginRequired.o \
	${OBJECTDIR}/IMsgInLoginUnwanted.o \
	${OBJECTDIR}/IMsgOut.o \
	${OBJECTDIR}/Logger.o \
	${OBJECTDIR}/MsgInAdapterListen.o \
	${OBJECTDIR}/MsgInAddAccount.o \
	${OBJECTDIR}/MsgInAddAdapter.o \
	${OBJECTDIR}/MsgInAddRoom.o \
	${OBJECTDIR}/MsgInAddView.o \
	${OBJECTDIR}/MsgInChangeConAccount.o \
	${OBJECTDIR}/MsgInDelConAccount.o \
	${OBJECTDIR}/MsgInDelCondition.o \
	${OBJECTDIR}/MsgInDelDevice.o \
	${OBJECTDIR}/MsgInDelRoom.o \
	${OBJECTDIR}/MsgInDelView.o \
	${OBJECTDIR}/MsgInDevices.o \
	${OBJECTDIR}/MsgInFactory.o \
	${OBJECTDIR}/MsgInGetActions.o \
	${OBJECTDIR}/MsgInGetAdapters.o \
	${OBJECTDIR}/MsgInGetAllDevices.o \
	${OBJECTDIR}/MsgInGetConAccount.o \
	${OBJECTDIR}/MsgInGetConditions.o \
	${OBJECTDIR}/MsgInGetRooms.o \
	${OBJECTDIR}/MsgInGetTimeZone.o \
	${OBJECTDIR}/MsgInGetViews.o \
	${OBJECTDIR}/MsgInInit.o \
	${OBJECTDIR}/MsgInLogName.o \
	${OBJECTDIR}/MsgInReinitAdapter.o \
	${OBJECTDIR}/MsgInSetAction.o \
	${OBJECTDIR}/MsgInSetCondition.o \
	${OBJECTDIR}/MsgInSetTimeZone.o \
	${OBJECTDIR}/MsgInSignIn.o \
	${OBJECTDIR}/MsgInSignUp.o \
	${OBJECTDIR}/MsgInSwitch.o \
	${OBJECTDIR}/MsgInUnknown.o \
	${OBJECTDIR}/MsgInUpdate.o \
	${OBJECTDIR}/MsgInUpdateCondition.o \
	${OBJECTDIR}/MsgInUpdateRooms.o \
	${OBJECTDIR}/MsgInUpdateView.o \
	${OBJECTDIR}/MsgOutAccountList.o \
	${OBJECTDIR}/MsgOutActionCreated.o \
	${OBJECTDIR}/MsgOutAdaptersReady.o \
	${OBJECTDIR}/MsgOutAllDevices.o \
	${OBJECTDIR}/MsgOutCondition.o \
	${OBJECTDIR}/MsgOutConditionCreated.o \
	${OBJECTDIR}/MsgOutConditions.o \
	${OBJECTDIR}/MsgOutFalse.o \
	${OBJECTDIR}/MsgOutLogContent.o \
	${OBJECTDIR}/MsgOutNotRegA.o \
	${OBJECTDIR}/MsgOutNotRegB.o \
	${OBJECTDIR}/MsgOutPartial.o \
	${OBJECTDIR}/MsgOutReSign.o \
	${OBJECTDIR}/MsgOutRoomCreated.o \
	${OBJECTDIR}/MsgOutRooms.o \
	${OBJECTDIR}/MsgOutTimeZone.o \
	${OBJECTDIR}/MsgOutTrue.o \
	${OBJECTDIR}/MsgOutViewsList.o \
	${OBJECTDIR}/MsgRightsChecker.o \
	${OBJECTDIR}/Server.o \
	${OBJECTDIR}/ServerException.o \
	${OBJECTDIR}/SocketClient.o \
	${OBJECTDIR}/ccolor.o \
	${OBJECTDIR}/communication.o \
	${OBJECTDIR}/gTokenChecker.o \
	${OBJECTDIR}/msgInGetCondition.o \
	${OBJECTDIR}/pugixml.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f21 \
	${TESTDIR}/TestFiles/f15 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f23 \
	${TESTDIR}/TestFiles/f20 \
	${TESTDIR}/TestFiles/f8 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f16 \
	${TESTDIR}/TestFiles/f9 \
	${TESTDIR}/TestFiles/f26 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f19 \
	${TESTDIR}/TestFiles/f18 \
	${TESTDIR}/TestFiles/f17 \
	${TESTDIR}/TestFiles/f24 \
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
	${TESTDIR}/TestFiles/f25 \
	${TESTDIR}/TestFiles/f6

# C Compiler Flags
CFLAGS=`cppunit-config --cflags` 

# CC Compiler Flags
CCFLAGS=`cppunit-config --cflags` -std=c++0x 
CXXFLAGS=`cppunit-config --cflags` -std=c++0x 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../soci-3.2.2/build/lib -L/usr/local/include/soci -L../soci-3.2.2/soci-3.2.2/backends/postgresql -L../soci-3.2.2/soci-3.2.2/core -L/usr/include/postgresql -L/usr/include/postgresql/libpq -L. -L/usr/include -L/usr/local/include -L/usr/local/lib -L/usr/lib -Wl,-rpath,../soci-3.2.2/build/lib -Wl,-rpath,/usr/local/lib `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` -lpq `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs` `cppunit-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server ${OBJECTFILES} ${LDLIBSOPTIONS} -lpq -lsoci_core -lsoci_empty -lsoci_postgresql -lcppunit -lssl -lcrypto -ljansson

${OBJECTDIR}/ComTable.o: ComTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComTable.o ComTable.cpp

${OBJECTDIR}/Config.o: Config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config.o Config.cpp

${OBJECTDIR}/DBConnector.o: DBConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector.o DBConnector.cpp

${OBJECTDIR}/IMsgIn.o: IMsgIn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgIn.o IMsgIn.cpp

${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o: IMsgInLoginAndAdapterAccessRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired.o IMsgInLoginAndAdapterAccessRequired.cpp

${OBJECTDIR}/IMsgInLoginRequired.o: IMsgInLoginRequired.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginRequired.o IMsgInLoginRequired.cpp

${OBJECTDIR}/IMsgInLoginUnwanted.o: IMsgInLoginUnwanted.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginUnwanted.o IMsgInLoginUnwanted.cpp

${OBJECTDIR}/IMsgOut.o: IMsgOut.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgOut.o IMsgOut.cpp

${OBJECTDIR}/Logger.o: Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Logger.o Logger.cpp

${OBJECTDIR}/MsgInAdapterListen.o: MsgInAdapterListen.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAdapterListen.o MsgInAdapterListen.cpp

${OBJECTDIR}/MsgInAddAccount.o: MsgInAddAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAccount.o MsgInAddAccount.cpp

${OBJECTDIR}/MsgInAddAdapter.o: MsgInAddAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAdapter.o MsgInAddAdapter.cpp

${OBJECTDIR}/MsgInAddRoom.o: MsgInAddRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddRoom.o MsgInAddRoom.cpp

${OBJECTDIR}/MsgInAddView.o: MsgInAddView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddView.o MsgInAddView.cpp

${OBJECTDIR}/MsgInChangeConAccount.o: MsgInChangeConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInChangeConAccount.o MsgInChangeConAccount.cpp

${OBJECTDIR}/MsgInDelConAccount.o: MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelConAccount.o MsgInDelConAccount.cpp

${OBJECTDIR}/MsgInDelCondition.o: MsgInDelCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelCondition.o MsgInDelCondition.cpp

${OBJECTDIR}/MsgInDelDevice.o: MsgInDelDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelDevice.o MsgInDelDevice.cpp

${OBJECTDIR}/MsgInDelRoom.o: MsgInDelRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelRoom.o MsgInDelRoom.cpp

${OBJECTDIR}/MsgInDelView.o: MsgInDelView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelView.o MsgInDelView.cpp

${OBJECTDIR}/MsgInDevices.o: MsgInDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDevices.o MsgInDevices.cpp

${OBJECTDIR}/MsgInFactory.o: MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp

${OBJECTDIR}/MsgInGetActions.o: MsgInGetActions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetActions.o MsgInGetActions.cpp

${OBJECTDIR}/MsgInGetAdapters.o: MsgInGetAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAdapters.o MsgInGetAdapters.cpp

${OBJECTDIR}/MsgInGetAllDevices.o: MsgInGetAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAllDevices.o MsgInGetAllDevices.cpp

${OBJECTDIR}/MsgInGetConAccount.o: MsgInGetConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetConAccount.o MsgInGetConAccount.cpp

${OBJECTDIR}/MsgInGetConditions.o: MsgInGetConditions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetConditions.o MsgInGetConditions.cpp

${OBJECTDIR}/MsgInGetRooms.o: MsgInGetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetRooms.o MsgInGetRooms.cpp

${OBJECTDIR}/MsgInGetTimeZone.o: MsgInGetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetTimeZone.o MsgInGetTimeZone.cpp

${OBJECTDIR}/MsgInGetViews.o: MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetViews.o MsgInGetViews.cpp

${OBJECTDIR}/MsgInInit.o: MsgInInit.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInInit.o MsgInInit.cpp

${OBJECTDIR}/MsgInLogName.o: MsgInLogName.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInLogName.o MsgInLogName.cpp

${OBJECTDIR}/MsgInReinitAdapter.o: MsgInReinitAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInReinitAdapter.o MsgInReinitAdapter.cpp

${OBJECTDIR}/MsgInSetAction.o: MsgInSetAction.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetAction.o MsgInSetAction.cpp

${OBJECTDIR}/MsgInSetCondition.o: MsgInSetCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetCondition.o MsgInSetCondition.cpp

${OBJECTDIR}/MsgInSetTimeZone.o: MsgInSetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetTimeZone.o MsgInSetTimeZone.cpp

${OBJECTDIR}/MsgInSignIn.o: MsgInSignIn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSignIn.o MsgInSignIn.cpp

${OBJECTDIR}/MsgInSignUp.o: MsgInSignUp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSignUp.o MsgInSignUp.cpp

${OBJECTDIR}/MsgInSwitch.o: MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSwitch.o MsgInSwitch.cpp

${OBJECTDIR}/MsgInUnknown.o: MsgInUnknown.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUnknown.o MsgInUnknown.cpp

${OBJECTDIR}/MsgInUpdate.o: MsgInUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdate.o MsgInUpdate.cpp

${OBJECTDIR}/MsgInUpdateCondition.o: MsgInUpdateCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateCondition.o MsgInUpdateCondition.cpp

${OBJECTDIR}/MsgInUpdateRooms.o: MsgInUpdateRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateRooms.o MsgInUpdateRooms.cpp

${OBJECTDIR}/MsgInUpdateView.o: MsgInUpdateView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateView.o MsgInUpdateView.cpp

${OBJECTDIR}/MsgOutAccountList.o: MsgOutAccountList.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutAccountList.o MsgOutAccountList.cpp

${OBJECTDIR}/MsgOutActionCreated.o: MsgOutActionCreated.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutActionCreated.o MsgOutActionCreated.cpp

${OBJECTDIR}/MsgOutAdaptersReady.o: MsgOutAdaptersReady.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutAdaptersReady.o MsgOutAdaptersReady.cpp

${OBJECTDIR}/MsgOutAllDevices.o: MsgOutAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutAllDevices.o MsgOutAllDevices.cpp

${OBJECTDIR}/MsgOutCondition.o: MsgOutCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutCondition.o MsgOutCondition.cpp

${OBJECTDIR}/MsgOutConditionCreated.o: MsgOutConditionCreated.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutConditionCreated.o MsgOutConditionCreated.cpp

${OBJECTDIR}/MsgOutConditions.o: MsgOutConditions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutConditions.o MsgOutConditions.cpp

${OBJECTDIR}/MsgOutFalse.o: MsgOutFalse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutFalse.o MsgOutFalse.cpp

${OBJECTDIR}/MsgOutLogContent.o: MsgOutLogContent.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutLogContent.o MsgOutLogContent.cpp

${OBJECTDIR}/MsgOutNotRegA.o: MsgOutNotRegA.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutNotRegA.o MsgOutNotRegA.cpp

${OBJECTDIR}/MsgOutNotRegB.o: MsgOutNotRegB.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutNotRegB.o MsgOutNotRegB.cpp

${OBJECTDIR}/MsgOutPartial.o: MsgOutPartial.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutPartial.o MsgOutPartial.cpp

${OBJECTDIR}/MsgOutReSign.o: MsgOutReSign.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutReSign.o MsgOutReSign.cpp

${OBJECTDIR}/MsgOutRoomCreated.o: MsgOutRoomCreated.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutRoomCreated.o MsgOutRoomCreated.cpp

${OBJECTDIR}/MsgOutRooms.o: MsgOutRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutRooms.o MsgOutRooms.cpp

${OBJECTDIR}/MsgOutTimeZone.o: MsgOutTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutTimeZone.o MsgOutTimeZone.cpp

${OBJECTDIR}/MsgOutTrue.o: MsgOutTrue.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutTrue.o MsgOutTrue.cpp

${OBJECTDIR}/MsgOutViewsList.o: MsgOutViewsList.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutViewsList.o MsgOutViewsList.cpp

${OBJECTDIR}/MsgRightsChecker.o: MsgRightsChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgRightsChecker.o MsgRightsChecker.cpp

${OBJECTDIR}/Server.o: Server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server.o Server.cpp

${OBJECTDIR}/ServerException.o: ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException.o ServerException.cpp

${OBJECTDIR}/SocketClient.o: SocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient.o SocketClient.cpp

${OBJECTDIR}/ccolor.o: ccolor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccolor.o ccolor.cpp

${OBJECTDIR}/communication.o: communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication.o communication.cpp

${OBJECTDIR}/gTokenChecker.o: gTokenChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker.o gTokenChecker.cpp

${OBJECTDIR}/msgInGetCondition.o: msgInGetCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgInGetCondition.o msgInGetCondition.cpp

${OBJECTDIR}/pugixml.o: pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml.o pugixml.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f21: ${TESTDIR}/tests/newtestclass.o ${TESTDIR}/tests/newtestrunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f21 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f15: ${TESTDIR}/tests/testChangeConAccount.o ${TESTDIR}/tests/testrunnerChangeConAccount.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f15 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/testGlobal.o ${TESTDIR}/tests/testrunnerGlobal.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f23: ${TESTDIR}/tests/testMsgGetCondition.o ${TESTDIR}/tests/testrunnerMsgGetCondition.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f23 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f20: ${TESTDIR}/tests/testMsgInAddAdapter.o ${TESTDIR}/tests/testrunnerMsgInAddAdapter.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f20 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f8: ${TESTDIR}/tests/testMsgInAddConAcc.o ${TESTDIR}/tests/testrunnerMsgInAddConAcc.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f8 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/testMsgInAddRooms.o ${TESTDIR}/tests/testrunnerMsgInAddRooms.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f16: ${TESTDIR}/tests/testMsgInDelConAcc.o ${TESTDIR}/tests/testrunnerMsgInDelConAcc.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f16 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f9: ${TESTDIR}/tests/testMsgInDelDevice.o ${TESTDIR}/tests/testrunnerMsgInDelDevice.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f9 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f26: ${TESTDIR}/tests/testMsgInDeleteCondition.o ${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f26 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/testMsgInDelRoom.o ${TESTDIR}/tests/testrunnerMsgInDelRoom.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f19: ${TESTDIR}/tests/testMsgInPartial.o ${TESTDIR}/tests/testrunnerMsgInPartial.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f19 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f18: ${TESTDIR}/tests/testMsgInGetAdapters.o ${TESTDIR}/tests/testrunnerMsgInGetAdapters.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f18 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f17: ${TESTDIR}/tests/testMsgInGetConAcc.o ${TESTDIR}/tests/testrunnerMsgInGetConAcc.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f17 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f24: ${TESTDIR}/tests/testMsgInGetConditions.o ${TESTDIR}/tests/testrunnerMsgInGetConditions.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f24 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f7: ${TESTDIR}/tests/testMsgInGetRooms.o ${TESTDIR}/tests/testrunnerMsgInGetRooms.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f14: ${TESTDIR}/tests/testMsgInGetTimeZone.o ${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f14 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f11: ${TESTDIR}/tests/testMsgInLogName.o ${TESTDIR}/tests/testrunnerMsgInLogName.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f11 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f27: ${TESTDIR}/tests/testMsgInSetAction.o ${TESTDIR}/tests/testrunnerMsgInSetAction.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f27 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f22: ${TESTDIR}/tests/testMsgInSetCondition.o ${TESTDIR}/tests/testrunnerMsgInSetCondition.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f22 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f13: ${TESTDIR}/tests/testMsgInSetTimeZone.o ${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f13 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/testMsgInSignIn.o ${TESTDIR}/tests/testrunnerMsgInSignIn.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/testMsgInSignUp.o ${TESTDIR}/tests/testrunnerMsgInSignUp.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` `cppunit-config --libs`   

${TESTDIR}/TestFiles/f10: ${TESTDIR}/tests/testMsgInSwitch.o ${TESTDIR}/tests/testrunnerMsgInSwitch.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f10 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f12: ${TESTDIR}/tests/testMsgInUpdate.o ${TESTDIR}/tests/testrunnerMsgInUpdate.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f12 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f25: ${TESTDIR}/tests/testMsgInUpdateCondition.o ${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f25 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests/testMsgInUpdateRooms.o ${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/tests/newtestclass.o: tests/newtestclass.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestclass.o tests/newtestclass.cpp


${TESTDIR}/tests/newtestrunner.o: tests/newtestrunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestrunner.o tests/newtestrunner.cpp


${TESTDIR}/tests/testChangeConAccount.o: tests/testChangeConAccount.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testChangeConAccount.o tests/testChangeConAccount.cpp


${TESTDIR}/tests/testrunnerChangeConAccount.o: tests/testrunnerChangeConAccount.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerChangeConAccount.o tests/testrunnerChangeConAccount.cpp


${TESTDIR}/tests/testGlobal.o: tests/testGlobal.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testGlobal.o tests/testGlobal.cpp


${TESTDIR}/tests/testrunnerGlobal.o: tests/testrunnerGlobal.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerGlobal.o tests/testrunnerGlobal.cpp


${TESTDIR}/tests/testMsgGetCondition.o: tests/testMsgGetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgGetCondition.o tests/testMsgGetCondition.cpp


${TESTDIR}/tests/testrunnerMsgGetCondition.o: tests/testrunnerMsgGetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgGetCondition.o tests/testrunnerMsgGetCondition.cpp


${TESTDIR}/tests/testMsgInAddAdapter.o: tests/testMsgInAddAdapter.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddAdapter.o tests/testMsgInAddAdapter.cpp


${TESTDIR}/tests/testrunnerMsgInAddAdapter.o: tests/testrunnerMsgInAddAdapter.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddAdapter.o tests/testrunnerMsgInAddAdapter.cpp


${TESTDIR}/tests/testMsgInAddConAcc.o: tests/testMsgInAddConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddConAcc.o tests/testMsgInAddConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInAddConAcc.o: tests/testrunnerMsgInAddConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddConAcc.o tests/testrunnerMsgInAddConAcc.cpp


${TESTDIR}/tests/testMsgInAddRooms.o: tests/testMsgInAddRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInAddRooms.o tests/testMsgInAddRooms.cpp


${TESTDIR}/tests/testrunnerMsgInAddRooms.o: tests/testrunnerMsgInAddRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInAddRooms.o tests/testrunnerMsgInAddRooms.cpp


${TESTDIR}/tests/testMsgInDelConAcc.o: tests/testMsgInDelConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelConAcc.o tests/testMsgInDelConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInDelConAcc.o: tests/testrunnerMsgInDelConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelConAcc.o tests/testrunnerMsgInDelConAcc.cpp


${TESTDIR}/tests/testMsgInDelDevice.o: tests/testMsgInDelDevice.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelDevice.o tests/testMsgInDelDevice.cpp


${TESTDIR}/tests/testrunnerMsgInDelDevice.o: tests/testrunnerMsgInDelDevice.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelDevice.o tests/testrunnerMsgInDelDevice.cpp


${TESTDIR}/tests/testMsgInDeleteCondition.o: tests/testMsgInDeleteCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDeleteCondition.o tests/testMsgInDeleteCondition.cpp


${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o: tests/testrunnerMsgInDeleteCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDeleteCondition.o tests/testrunnerMsgInDeleteCondition.cpp


${TESTDIR}/tests/testMsgInDelRoom.o: tests/testMsgInDelRoom.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInDelRoom.o tests/testMsgInDelRoom.cpp


${TESTDIR}/tests/testrunnerMsgInDelRoom.o: tests/testrunnerMsgInDelRoom.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInDelRoom.o tests/testrunnerMsgInDelRoom.cpp


${TESTDIR}/tests/testMsgInPartial.o: tests/testMsgInPartial.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInPartial.o tests/testMsgInPartial.cpp


${TESTDIR}/tests/testrunnerMsgInPartial.o: tests/testrunnerMsgInPartial.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInPartial.o tests/testrunnerMsgInPartial.cpp


${TESTDIR}/tests/testMsgInGetAdapters.o: tests/testMsgInGetAdapters.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetAdapters.o tests/testMsgInGetAdapters.cpp


${TESTDIR}/tests/testrunnerMsgInGetAdapters.o: tests/testrunnerMsgInGetAdapters.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetAdapters.o tests/testrunnerMsgInGetAdapters.cpp


${TESTDIR}/tests/testMsgInGetConAcc.o: tests/testMsgInGetConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetConAcc.o tests/testMsgInGetConAcc.cpp


${TESTDIR}/tests/testrunnerMsgInGetConAcc.o: tests/testrunnerMsgInGetConAcc.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetConAcc.o tests/testrunnerMsgInGetConAcc.cpp


${TESTDIR}/tests/testMsgInGetConditions.o: tests/testMsgInGetConditions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetConditions.o tests/testMsgInGetConditions.cpp


${TESTDIR}/tests/testrunnerMsgInGetConditions.o: tests/testrunnerMsgInGetConditions.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetConditions.o tests/testrunnerMsgInGetConditions.cpp


${TESTDIR}/tests/testMsgInGetRooms.o: tests/testMsgInGetRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetRooms.o tests/testMsgInGetRooms.cpp


${TESTDIR}/tests/testrunnerMsgInGetRooms.o: tests/testrunnerMsgInGetRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetRooms.o tests/testrunnerMsgInGetRooms.cpp


${TESTDIR}/tests/testMsgInGetTimeZone.o: tests/testMsgInGetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInGetTimeZone.o tests/testMsgInGetTimeZone.cpp


${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o: tests/testrunnerMsgInGetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInGetTimeZone.o tests/testrunnerMsgInGetTimeZone.cpp


${TESTDIR}/tests/testMsgInLogName.o: tests/testMsgInLogName.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInLogName.o tests/testMsgInLogName.cpp


${TESTDIR}/tests/testrunnerMsgInLogName.o: tests/testrunnerMsgInLogName.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInLogName.o tests/testrunnerMsgInLogName.cpp


${TESTDIR}/tests/testMsgInSetAction.o: tests/testMsgInSetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetAction.o tests/testMsgInSetAction.cpp


${TESTDIR}/tests/testrunnerMsgInSetAction.o: tests/testrunnerMsgInSetAction.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetAction.o tests/testrunnerMsgInSetAction.cpp


${TESTDIR}/tests/testMsgInSetCondition.o: tests/testMsgInSetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetCondition.o tests/testMsgInSetCondition.cpp


${TESTDIR}/tests/testrunnerMsgInSetCondition.o: tests/testrunnerMsgInSetCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetCondition.o tests/testrunnerMsgInSetCondition.cpp


${TESTDIR}/tests/testMsgInSetTimeZone.o: tests/testMsgInSetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSetTimeZone.o tests/testMsgInSetTimeZone.cpp


${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o: tests/testrunnerMsgInSetTimeZone.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSetTimeZone.o tests/testrunnerMsgInSetTimeZone.cpp


${TESTDIR}/tests/testMsgInSignIn.o: tests/testMsgInSignIn.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSignIn.o tests/testMsgInSignIn.cpp


${TESTDIR}/tests/testrunnerMsgInSignIn.o: tests/testrunnerMsgInSignIn.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSignIn.o tests/testrunnerMsgInSignIn.cpp


${TESTDIR}/tests/testMsgInSignUp.o: tests/testMsgInSignUp.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSignUp.o tests/testMsgInSignUp.cpp


${TESTDIR}/tests/testrunnerMsgInSignUp.o: tests/testrunnerMsgInSignUp.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSignUp.o tests/testrunnerMsgInSignUp.cpp


${TESTDIR}/tests/testMsgInSwitch.o: tests/testMsgInSwitch.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInSwitch.o tests/testMsgInSwitch.cpp


${TESTDIR}/tests/testrunnerMsgInSwitch.o: tests/testrunnerMsgInSwitch.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInSwitch.o tests/testrunnerMsgInSwitch.cpp


${TESTDIR}/tests/testMsgInUpdate.o: tests/testMsgInUpdate.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdate.o tests/testMsgInUpdate.cpp


${TESTDIR}/tests/testrunnerMsgInUpdate.o: tests/testrunnerMsgInUpdate.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdate.o tests/testrunnerMsgInUpdate.cpp


${TESTDIR}/tests/testMsgInUpdateCondition.o: tests/testMsgInUpdateCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateCondition.o tests/testMsgInUpdateCondition.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o: tests/testrunnerMsgInUpdateCondition.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateCondition.o tests/testrunnerMsgInUpdateCondition.cpp


${TESTDIR}/tests/testMsgInUpdateRooms.o: tests/testMsgInUpdateRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testMsgInUpdateRooms.o tests/testMsgInUpdateRooms.cpp


${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o: tests/testrunnerMsgInUpdateRooms.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerMsgInUpdateRooms.o tests/testrunnerMsgInUpdateRooms.cpp


${OBJECTDIR}/ComTable_nomain.o: ${OBJECTDIR}/ComTable.o ComTable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ComTable.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComTable_nomain.o ComTable.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config_nomain.o Config.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnector_nomain.o DBConnector.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgIn_nomain.o IMsgIn.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginAndAdapterAccessRequired_nomain.o IMsgInLoginAndAdapterAccessRequired.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginRequired_nomain.o IMsgInLoginRequired.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgInLoginUnwanted_nomain.o IMsgInLoginUnwanted.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/IMsgInLoginUnwanted.o ${OBJECTDIR}/IMsgInLoginUnwanted_nomain.o;\
	fi

${OBJECTDIR}/IMsgOut_nomain.o: ${OBJECTDIR}/IMsgOut.o IMsgOut.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/IMsgOut.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IMsgOut_nomain.o IMsgOut.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/IMsgOut.o ${OBJECTDIR}/IMsgOut_nomain.o;\
	fi

${OBJECTDIR}/Logger_nomain.o: ${OBJECTDIR}/Logger.o Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Logger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Logger_nomain.o Logger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Logger.o ${OBJECTDIR}/Logger_nomain.o;\
	fi

${OBJECTDIR}/MsgInAdapterListen_nomain.o: ${OBJECTDIR}/MsgInAdapterListen.o MsgInAdapterListen.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInAdapterListen.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAdapterListen_nomain.o MsgInAdapterListen.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAccount_nomain.o MsgInAddAccount.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddAdapter_nomain.o MsgInAddAdapter.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddRoom_nomain.o MsgInAddRoom.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInAddView_nomain.o MsgInAddView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInAddView.o ${OBJECTDIR}/MsgInAddView_nomain.o;\
	fi

${OBJECTDIR}/MsgInChangeConAccount_nomain.o: ${OBJECTDIR}/MsgInChangeConAccount.o MsgInChangeConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInChangeConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInChangeConAccount_nomain.o MsgInChangeConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInChangeConAccount.o ${OBJECTDIR}/MsgInChangeConAccount_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelConAccount_nomain.o: ${OBJECTDIR}/MsgInDelConAccount.o MsgInDelConAccount.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelConAccount.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelConAccount_nomain.o MsgInDelConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelConAccount.o ${OBJECTDIR}/MsgInDelConAccount_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelCondition_nomain.o: ${OBJECTDIR}/MsgInDelCondition.o MsgInDelCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelCondition.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelCondition_nomain.o MsgInDelCondition.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelCondition.o ${OBJECTDIR}/MsgInDelCondition_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelDevice_nomain.o: ${OBJECTDIR}/MsgInDelDevice.o MsgInDelDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelDevice.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelDevice_nomain.o MsgInDelDevice.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelDevice.o ${OBJECTDIR}/MsgInDelDevice_nomain.o;\
	fi

${OBJECTDIR}/MsgInDelRoom_nomain.o: ${OBJECTDIR}/MsgInDelRoom.o MsgInDelRoom.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDelRoom.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelRoom_nomain.o MsgInDelRoom.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDelView_nomain.o MsgInDelView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDelView.o ${OBJECTDIR}/MsgInDelView_nomain.o;\
	fi

${OBJECTDIR}/MsgInDevices_nomain.o: ${OBJECTDIR}/MsgInDevices.o MsgInDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInDevices_nomain.o MsgInDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInDevices.o ${OBJECTDIR}/MsgInDevices_nomain.o;\
	fi

${OBJECTDIR}/MsgInFactory_nomain.o: ${OBJECTDIR}/MsgInFactory.o MsgInFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInFactory.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInFactory_nomain.o MsgInFactory.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInFactory.o ${OBJECTDIR}/MsgInFactory_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetActions_nomain.o: ${OBJECTDIR}/MsgInGetActions.o MsgInGetActions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetActions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetActions_nomain.o MsgInGetActions.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetActions.o ${OBJECTDIR}/MsgInGetActions_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetAdapters_nomain.o: ${OBJECTDIR}/MsgInGetAdapters.o MsgInGetAdapters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetAdapters.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAdapters_nomain.o MsgInGetAdapters.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetAllDevices_nomain.o MsgInGetAllDevices.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetConAccount_nomain.o MsgInGetConAccount.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetConAccount.o ${OBJECTDIR}/MsgInGetConAccount_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetConditions_nomain.o: ${OBJECTDIR}/MsgInGetConditions.o MsgInGetConditions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetConditions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetConditions_nomain.o MsgInGetConditions.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetConditions.o ${OBJECTDIR}/MsgInGetConditions_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetRooms_nomain.o: ${OBJECTDIR}/MsgInGetRooms.o MsgInGetRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetRooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetRooms_nomain.o MsgInGetRooms.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetTimeZone_nomain.o MsgInGetTimeZone.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetTimeZone.o ${OBJECTDIR}/MsgInGetTimeZone_nomain.o;\
	fi

${OBJECTDIR}/MsgInGetViews_nomain.o: ${OBJECTDIR}/MsgInGetViews.o MsgInGetViews.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInGetViews.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInGetViews_nomain.o MsgInGetViews.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInGetViews.o ${OBJECTDIR}/MsgInGetViews_nomain.o;\
	fi

${OBJECTDIR}/MsgInInit_nomain.o: ${OBJECTDIR}/MsgInInit.o MsgInInit.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInInit.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInInit_nomain.o MsgInInit.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInInit.o ${OBJECTDIR}/MsgInInit_nomain.o;\
	fi

${OBJECTDIR}/MsgInLogName_nomain.o: ${OBJECTDIR}/MsgInLogName.o MsgInLogName.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInLogName.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInLogName_nomain.o MsgInLogName.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInLogName.o ${OBJECTDIR}/MsgInLogName_nomain.o;\
	fi

${OBJECTDIR}/MsgInReinitAdapter_nomain.o: ${OBJECTDIR}/MsgInReinitAdapter.o MsgInReinitAdapter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInReinitAdapter.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInReinitAdapter_nomain.o MsgInReinitAdapter.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInReinitAdapter.o ${OBJECTDIR}/MsgInReinitAdapter_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetAction_nomain.o: ${OBJECTDIR}/MsgInSetAction.o MsgInSetAction.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetAction.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetAction_nomain.o MsgInSetAction.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetAction.o ${OBJECTDIR}/MsgInSetAction_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetCondition_nomain.o: ${OBJECTDIR}/MsgInSetCondition.o MsgInSetCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetCondition.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetCondition_nomain.o MsgInSetCondition.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetCondition.o ${OBJECTDIR}/MsgInSetCondition_nomain.o;\
	fi

${OBJECTDIR}/MsgInSetTimeZone_nomain.o: ${OBJECTDIR}/MsgInSetTimeZone.o MsgInSetTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSetTimeZone.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSetTimeZone_nomain.o MsgInSetTimeZone.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSetTimeZone.o ${OBJECTDIR}/MsgInSetTimeZone_nomain.o;\
	fi

${OBJECTDIR}/MsgInSignIn_nomain.o: ${OBJECTDIR}/MsgInSignIn.o MsgInSignIn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSignIn.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSignIn_nomain.o MsgInSignIn.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSignIn.o ${OBJECTDIR}/MsgInSignIn_nomain.o;\
	fi

${OBJECTDIR}/MsgInSignUp_nomain.o: ${OBJECTDIR}/MsgInSignUp.o MsgInSignUp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSignUp.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSignUp_nomain.o MsgInSignUp.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInSignUp.o ${OBJECTDIR}/MsgInSignUp_nomain.o;\
	fi

${OBJECTDIR}/MsgInSwitch_nomain.o: ${OBJECTDIR}/MsgInSwitch.o MsgInSwitch.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInSwitch.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInSwitch_nomain.o MsgInSwitch.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUnknown_nomain.o MsgInUnknown.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInUnknown.o ${OBJECTDIR}/MsgInUnknown_nomain.o;\
	fi

${OBJECTDIR}/MsgInUpdate_nomain.o: ${OBJECTDIR}/MsgInUpdate.o MsgInUpdate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInUpdate.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdate_nomain.o MsgInUpdate.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInUpdate.o ${OBJECTDIR}/MsgInUpdate_nomain.o;\
	fi

${OBJECTDIR}/MsgInUpdateCondition_nomain.o: ${OBJECTDIR}/MsgInUpdateCondition.o MsgInUpdateCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInUpdateCondition.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateCondition_nomain.o MsgInUpdateCondition.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInUpdateCondition.o ${OBJECTDIR}/MsgInUpdateCondition_nomain.o;\
	fi

${OBJECTDIR}/MsgInUpdateRooms_nomain.o: ${OBJECTDIR}/MsgInUpdateRooms.o MsgInUpdateRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInUpdateRooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateRooms_nomain.o MsgInUpdateRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInUpdateRooms.o ${OBJECTDIR}/MsgInUpdateRooms_nomain.o;\
	fi

${OBJECTDIR}/MsgInUpdateView_nomain.o: ${OBJECTDIR}/MsgInUpdateView.o MsgInUpdateView.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgInUpdateView.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgInUpdateView_nomain.o MsgInUpdateView.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgInUpdateView.o ${OBJECTDIR}/MsgInUpdateView_nomain.o;\
	fi

${OBJECTDIR}/MsgOutAccountList_nomain.o: ${OBJECTDIR}/MsgOutAccountList.o MsgOutAccountList.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutAccountList.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutAccountList_nomain.o MsgOutAccountList.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutAccountList.o ${OBJECTDIR}/MsgOutAccountList_nomain.o;\
	fi

${OBJECTDIR}/MsgOutActionCreated_nomain.o: ${OBJECTDIR}/MsgOutActionCreated.o MsgOutActionCreated.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutActionCreated.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutActionCreated_nomain.o MsgOutActionCreated.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutActionCreated.o ${OBJECTDIR}/MsgOutActionCreated_nomain.o;\
	fi

${OBJECTDIR}/MsgOutAdaptersReady_nomain.o: ${OBJECTDIR}/MsgOutAdaptersReady.o MsgOutAdaptersReady.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutAdaptersReady.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutAdaptersReady_nomain.o MsgOutAdaptersReady.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutAdaptersReady.o ${OBJECTDIR}/MsgOutAdaptersReady_nomain.o;\
	fi

${OBJECTDIR}/MsgOutAllDevices_nomain.o: ${OBJECTDIR}/MsgOutAllDevices.o MsgOutAllDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutAllDevices.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutAllDevices_nomain.o MsgOutAllDevices.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutAllDevices.o ${OBJECTDIR}/MsgOutAllDevices_nomain.o;\
	fi

${OBJECTDIR}/MsgOutCondition_nomain.o: ${OBJECTDIR}/MsgOutCondition.o MsgOutCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutCondition.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutCondition_nomain.o MsgOutCondition.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutCondition.o ${OBJECTDIR}/MsgOutCondition_nomain.o;\
	fi

${OBJECTDIR}/MsgOutConditionCreated_nomain.o: ${OBJECTDIR}/MsgOutConditionCreated.o MsgOutConditionCreated.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutConditionCreated.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutConditionCreated_nomain.o MsgOutConditionCreated.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutConditionCreated.o ${OBJECTDIR}/MsgOutConditionCreated_nomain.o;\
	fi

${OBJECTDIR}/MsgOutConditions_nomain.o: ${OBJECTDIR}/MsgOutConditions.o MsgOutConditions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutConditions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutConditions_nomain.o MsgOutConditions.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutConditions.o ${OBJECTDIR}/MsgOutConditions_nomain.o;\
	fi

${OBJECTDIR}/MsgOutFalse_nomain.o: ${OBJECTDIR}/MsgOutFalse.o MsgOutFalse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutFalse.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutFalse_nomain.o MsgOutFalse.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutFalse.o ${OBJECTDIR}/MsgOutFalse_nomain.o;\
	fi

${OBJECTDIR}/MsgOutLogContent_nomain.o: ${OBJECTDIR}/MsgOutLogContent.o MsgOutLogContent.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutLogContent.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutLogContent_nomain.o MsgOutLogContent.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutLogContent.o ${OBJECTDIR}/MsgOutLogContent_nomain.o;\
	fi

${OBJECTDIR}/MsgOutNotRegA_nomain.o: ${OBJECTDIR}/MsgOutNotRegA.o MsgOutNotRegA.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutNotRegA.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutNotRegA_nomain.o MsgOutNotRegA.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutNotRegA.o ${OBJECTDIR}/MsgOutNotRegA_nomain.o;\
	fi

${OBJECTDIR}/MsgOutNotRegB_nomain.o: ${OBJECTDIR}/MsgOutNotRegB.o MsgOutNotRegB.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutNotRegB.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutNotRegB_nomain.o MsgOutNotRegB.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutNotRegB.o ${OBJECTDIR}/MsgOutNotRegB_nomain.o;\
	fi

${OBJECTDIR}/MsgOutPartial_nomain.o: ${OBJECTDIR}/MsgOutPartial.o MsgOutPartial.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutPartial.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutPartial_nomain.o MsgOutPartial.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutPartial.o ${OBJECTDIR}/MsgOutPartial_nomain.o;\
	fi

${OBJECTDIR}/MsgOutReSign_nomain.o: ${OBJECTDIR}/MsgOutReSign.o MsgOutReSign.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutReSign.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutReSign_nomain.o MsgOutReSign.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutReSign.o ${OBJECTDIR}/MsgOutReSign_nomain.o;\
	fi

${OBJECTDIR}/MsgOutRoomCreated_nomain.o: ${OBJECTDIR}/MsgOutRoomCreated.o MsgOutRoomCreated.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutRoomCreated.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutRoomCreated_nomain.o MsgOutRoomCreated.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutRoomCreated.o ${OBJECTDIR}/MsgOutRoomCreated_nomain.o;\
	fi

${OBJECTDIR}/MsgOutRooms_nomain.o: ${OBJECTDIR}/MsgOutRooms.o MsgOutRooms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutRooms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutRooms_nomain.o MsgOutRooms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutRooms.o ${OBJECTDIR}/MsgOutRooms_nomain.o;\
	fi

${OBJECTDIR}/MsgOutTimeZone_nomain.o: ${OBJECTDIR}/MsgOutTimeZone.o MsgOutTimeZone.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutTimeZone.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutTimeZone_nomain.o MsgOutTimeZone.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutTimeZone.o ${OBJECTDIR}/MsgOutTimeZone_nomain.o;\
	fi

${OBJECTDIR}/MsgOutTrue_nomain.o: ${OBJECTDIR}/MsgOutTrue.o MsgOutTrue.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutTrue.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutTrue_nomain.o MsgOutTrue.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutTrue.o ${OBJECTDIR}/MsgOutTrue_nomain.o;\
	fi

${OBJECTDIR}/MsgOutViewsList_nomain.o: ${OBJECTDIR}/MsgOutViewsList.o MsgOutViewsList.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgOutViewsList.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgOutViewsList_nomain.o MsgOutViewsList.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgOutViewsList.o ${OBJECTDIR}/MsgOutViewsList_nomain.o;\
	fi

${OBJECTDIR}/MsgRightsChecker_nomain.o: ${OBJECTDIR}/MsgRightsChecker.o MsgRightsChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/MsgRightsChecker.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MsgRightsChecker_nomain.o MsgRightsChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/MsgRightsChecker.o ${OBJECTDIR}/MsgRightsChecker_nomain.o;\
	fi

${OBJECTDIR}/Server_nomain.o: ${OBJECTDIR}/Server.o Server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server_nomain.o Server.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Server.o ${OBJECTDIR}/Server_nomain.o;\
	fi

${OBJECTDIR}/ServerException_nomain.o: ${OBJECTDIR}/ServerException.o ServerException.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ServerException.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerException_nomain.o ServerException.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SocketClient_nomain.o SocketClient.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/SocketClient.o ${OBJECTDIR}/SocketClient_nomain.o;\
	fi

${OBJECTDIR}/ccolor_nomain.o: ${OBJECTDIR}/ccolor.o ccolor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ccolor.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccolor_nomain.o ccolor.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ccolor.o ${OBJECTDIR}/ccolor_nomain.o;\
	fi

${OBJECTDIR}/communication_nomain.o: ${OBJECTDIR}/communication.o communication.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/communication.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/communication_nomain.o communication.cpp;\
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
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gTokenChecker_nomain.o gTokenChecker.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/gTokenChecker.o ${OBJECTDIR}/gTokenChecker_nomain.o;\
	fi

${OBJECTDIR}/msgInGetCondition_nomain.o: ${OBJECTDIR}/msgInGetCondition.o msgInGetCondition.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/msgInGetCondition.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgInGetCondition_nomain.o msgInGetCondition.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/msgInGetCondition.o ${OBJECTDIR}/msgInGetCondition_nomain.o;\
	fi

${OBJECTDIR}/pugixml_nomain.o: ${OBJECTDIR}/pugixml.o pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/pugixml.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../soci-3.2.2/soci-3.2.2/backends/postgresql -I../soci-3.2.2/soci-3.2.2/core -I/usr/include/postgresql -I/usr/include/postgresql/libpq -I. -I/usr/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pugixml_nomain.o pugixml.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/pugixml.o ${OBJECTDIR}/pugixml_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f21 || true; \
	    ${TESTDIR}/TestFiles/f15 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f23 || true; \
	    ${TESTDIR}/TestFiles/f20 || true; \
	    ${TESTDIR}/TestFiles/f8 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f16 || true; \
	    ${TESTDIR}/TestFiles/f9 || true; \
	    ${TESTDIR}/TestFiles/f26 || true; \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f19 || true; \
	    ${TESTDIR}/TestFiles/f18 || true; \
	    ${TESTDIR}/TestFiles/f17 || true; \
	    ${TESTDIR}/TestFiles/f24 || true; \
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
	    ${TESTDIR}/TestFiles/f25 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
