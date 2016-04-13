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
CC=gcc-4.9
CCC=g++-4.9
CXX=g++-4.9
FC=gfortran
AS=as

# Macros
CND_PLATFORM=G++_4.9.3-Linux
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
	${OBJECTDIR}/src/BaseTaskLoader.o \
	${OBJECTDIR}/src/BaseTaskManager.o \
	${OBJECTDIR}/src/Calendar.o \
	${OBJECTDIR}/src/ConfigParser.o \
	${OBJECTDIR}/src/DataMessageParser.o \
	${OBJECTDIR}/src/DataMessageRegister.o \
	${OBJECTDIR}/src/DatabaseInterface.o \
	${OBJECTDIR}/src/GatewayServer.o \
	${OBJECTDIR}/src/GatewaySession.o \
	${OBJECTDIR}/src/Server.o \
	${OBJECTDIR}/src/Session.o \
	${OBJECTDIR}/src/Task.o \
	${OBJECTDIR}/src/TaskInstance.o \
	${OBJECTDIR}/src/TaskLoader.o \
	${OBJECTDIR}/src/TaskManager.o \
	${OBJECTDIR}/src/TimedTaskInstance.o \
	${OBJECTDIR}/src/TriggerTaskInstance.o \
	${OBJECTDIR}/src/UserMessageParser.o \
	${OBJECTDIR}/src/UserServer.o \
	${OBJECTDIR}/src/UserSession.o \
	${OBJECTDIR}/src/framework.o \
	${OBJECTDIR}/src/locked_stream.o \
	${OBJECTDIR}/src/pugixml.o \
	${OBJECTDIR}/src/unified_logger.o \
	${OBJECTDIR}/src/utility.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/BaseTaskLoader.o: src/BaseTaskLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/BaseTaskLoader.o src/BaseTaskLoader.cpp

${OBJECTDIR}/src/BaseTaskManager.o: src/BaseTaskManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/BaseTaskManager.o src/BaseTaskManager.cpp

${OBJECTDIR}/src/Calendar.o: src/Calendar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Calendar.o src/Calendar.cpp

${OBJECTDIR}/src/ConfigParser.o: src/ConfigParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ConfigParser.o src/ConfigParser.cpp

${OBJECTDIR}/src/DataMessageParser.o: src/DataMessageParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DataMessageParser.o src/DataMessageParser.cpp

${OBJECTDIR}/src/DataMessageRegister.o: src/DataMessageRegister.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DataMessageRegister.o src/DataMessageRegister.cpp

${OBJECTDIR}/src/DatabaseInterface.o: src/DatabaseInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DatabaseInterface.o src/DatabaseInterface.cpp

${OBJECTDIR}/src/GatewayServer.o: src/GatewayServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/GatewayServer.o src/GatewayServer.cpp

${OBJECTDIR}/src/GatewaySession.o: src/GatewaySession.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/GatewaySession.o src/GatewaySession.cpp

${OBJECTDIR}/src/Server.o: src/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Server.o src/Server.cpp

${OBJECTDIR}/src/Session.o: src/Session.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Session.o src/Session.cpp

${OBJECTDIR}/src/Task.o: src/Task.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Task.o src/Task.cpp

${OBJECTDIR}/src/TaskInstance.o: src/TaskInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TaskInstance.o src/TaskInstance.cpp

${OBJECTDIR}/src/TaskLoader.o: src/TaskLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TaskLoader.o src/TaskLoader.cpp

${OBJECTDIR}/src/TaskManager.o: src/TaskManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TaskManager.o src/TaskManager.cpp

${OBJECTDIR}/src/TimedTaskInstance.o: src/TimedTaskInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TimedTaskInstance.o src/TimedTaskInstance.cpp

${OBJECTDIR}/src/TriggerTaskInstance.o: src/TriggerTaskInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TriggerTaskInstance.o src/TriggerTaskInstance.cpp

${OBJECTDIR}/src/UserMessageParser.o: src/UserMessageParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserMessageParser.o src/UserMessageParser.cpp

${OBJECTDIR}/src/UserServer.o: src/UserServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserServer.o src/UserServer.cpp

${OBJECTDIR}/src/UserSession.o: src/UserSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UserSession.o src/UserSession.cpp

${OBJECTDIR}/src/framework.o: src/framework.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/framework.o src/framework.cpp

${OBJECTDIR}/src/locked_stream.o: src/locked_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/locked_stream.o src/locked_stream.cpp

${OBJECTDIR}/src/pugixml.o: src/pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pugixml.o src/pugixml.cpp

${OBJECTDIR}/src/unified_logger.o: src/unified_logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/unified_logger.o src/unified_logger.cpp

${OBJECTDIR}/src/utility.o: src/utility.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/utility.o src/utility.cpp

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
