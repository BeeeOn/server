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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/algorithms/RefreshInstance.o \
	${OBJECTDIR}/algorithms/RefreshManager.o \
	${OBJECTDIR}/algorithms/TestInstance.o \
	${OBJECTDIR}/algorithms/TestManager.o \
	${OBJECTDIR}/algorithms/WatchdogInstance.o \
	${OBJECTDIR}/algorithms/WatchdogManager.o \
	${OBJECTDIR}/src/AlgorithmInstance.o \
	${OBJECTDIR}/src/AlgorithmManager.o \
	${OBJECTDIR}/src/Calendar.o \
	${OBJECTDIR}/src/CalendarEvent.o \
	${OBJECTDIR}/src/ConfigMessage.o \
	${OBJECTDIR}/src/EventAlgorithmInstance.o \
	${OBJECTDIR}/src/EventAlgorithmManager.o \
	${OBJECTDIR}/src/Logger.o \
	${OBJECTDIR}/src/ManagerLoader.o \
	${OBJECTDIR}/src/MessageParser.o \
	${OBJECTDIR}/src/MessageRegister.o \
	${OBJECTDIR}/src/MessageSorter.o \
	${OBJECTDIR}/src/ModuleData.o \
	${OBJECTDIR}/src/Server.o \
	${OBJECTDIR}/src/ServerSession.o \
	${OBJECTDIR}/src/TimedAlgorithmInstance.o \
	${OBJECTDIR}/src/TimedAlgorithmManager.o \
	${OBJECTDIR}/src/framework.o \
	${OBJECTDIR}/src/u_server.o \
	${OBJECTDIR}/src/u_session.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++14 -pthread
CXXFLAGS=-std=c++14 -pthread

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/algorithms/RefreshInstance.o: algorithms/RefreshInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/algorithms
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/algorithms/RefreshInstance.o algorithms/RefreshInstance.cpp

${OBJECTDIR}/algorithms/RefreshManager.o: algorithms/RefreshManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/algorithms
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/algorithms/RefreshManager.o algorithms/RefreshManager.cpp

${OBJECTDIR}/algorithms/TestInstance.o: algorithms/TestInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/algorithms
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/algorithms/TestInstance.o algorithms/TestInstance.cpp

${OBJECTDIR}/algorithms/TestManager.o: algorithms/TestManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/algorithms
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/algorithms/TestManager.o algorithms/TestManager.cpp

${OBJECTDIR}/algorithms/WatchdogInstance.o: algorithms/WatchdogInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/algorithms
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/algorithms/WatchdogInstance.o algorithms/WatchdogInstance.cpp

${OBJECTDIR}/algorithms/WatchdogManager.o: algorithms/WatchdogManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/algorithms
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/algorithms/WatchdogManager.o algorithms/WatchdogManager.cpp

${OBJECTDIR}/src/AlgorithmInstance.o: src/AlgorithmInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AlgorithmInstance.o src/AlgorithmInstance.cpp

${OBJECTDIR}/src/AlgorithmManager.o: src/AlgorithmManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AlgorithmManager.o src/AlgorithmManager.cpp

${OBJECTDIR}/src/Calendar.o: src/Calendar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Calendar.o src/Calendar.cpp

${OBJECTDIR}/src/CalendarEvent.o: src/CalendarEvent.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CalendarEvent.o src/CalendarEvent.cpp

${OBJECTDIR}/src/ConfigMessage.o: src/ConfigMessage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ConfigMessage.o src/ConfigMessage.cpp

${OBJECTDIR}/src/EventAlgorithmInstance.o: src/EventAlgorithmInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EventAlgorithmInstance.o src/EventAlgorithmInstance.cpp

${OBJECTDIR}/src/EventAlgorithmManager.o: src/EventAlgorithmManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EventAlgorithmManager.o src/EventAlgorithmManager.cpp

${OBJECTDIR}/src/Logger.o: src/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Logger.o src/Logger.cpp

${OBJECTDIR}/src/ManagerLoader.o: src/ManagerLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ManagerLoader.o src/ManagerLoader.cpp

${OBJECTDIR}/src/MessageParser.o: src/MessageParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MessageParser.o src/MessageParser.cpp

${OBJECTDIR}/src/MessageRegister.o: src/MessageRegister.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MessageRegister.o src/MessageRegister.cpp

${OBJECTDIR}/src/MessageSorter.o: src/MessageSorter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MessageSorter.o src/MessageSorter.cpp

${OBJECTDIR}/src/ModuleData.o: src/ModuleData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ModuleData.o src/ModuleData.cpp

${OBJECTDIR}/src/Server.o: src/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Server.o src/Server.cpp

${OBJECTDIR}/src/ServerSession.o: src/ServerSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ServerSession.o src/ServerSession.cpp

${OBJECTDIR}/src/TimedAlgorithmInstance.o: src/TimedAlgorithmInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TimedAlgorithmInstance.o src/TimedAlgorithmInstance.cpp

${OBJECTDIR}/src/TimedAlgorithmManager.o: src/TimedAlgorithmManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TimedAlgorithmManager.o src/TimedAlgorithmManager.cpp

${OBJECTDIR}/src/framework.o: src/framework.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/framework.o src/framework.cpp

${OBJECTDIR}/src/u_server.o: src/u_server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/u_server.o src/u_server.cpp

${OBJECTDIR}/src/u_session.o: src/u_session.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/u_session.o src/u_session.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
