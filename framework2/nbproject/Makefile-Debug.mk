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
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=G++_4.9.3-Linux
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
	${OBJECTDIR}/src/Calendar.o \
	${OBJECTDIR}/src/CalendarEvent.o \
	${OBJECTDIR}/src/ConfigMessage.o \
	${OBJECTDIR}/src/ConfigParser.o \
	${OBJECTDIR}/src/MessageParser.o \
	${OBJECTDIR}/src/MessageRegister.o \
	${OBJECTDIR}/src/ModuleData.o \
	${OBJECTDIR}/src/Server.o \
	${OBJECTDIR}/src/ServerSession.o \
	${OBJECTDIR}/src/Task.o \
	${OBJECTDIR}/src/TaskInstance.o \
	${OBJECTDIR}/src/TaskLoader.o \
	${OBJECTDIR}/src/TaskManager.o \
	${OBJECTDIR}/src/TimedTaskInstance.o \
	${OBJECTDIR}/src/TriggerTaskInstance.o \
	${OBJECTDIR}/src/framework.o \
	${OBJECTDIR}/src/pugixml.o \
	${OBJECTDIR}/src/u_server.o \
	${OBJECTDIR}/src/u_session.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread -rdynamic
CXXFLAGS=-pthread -rdynamic

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib64 -L/usr/lib/postgresql/9.3/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/framework2 ${OBJECTFILES} ${LDLIBSOPTIONS} -lsoci_core -lsoci_empty -lsoci_postgresql -ldl -lpq

${OBJECTDIR}/src/Calendar.o: src/Calendar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Calendar.o src/Calendar.cpp

${OBJECTDIR}/src/CalendarEvent.o: src/CalendarEvent.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CalendarEvent.o src/CalendarEvent.cpp

${OBJECTDIR}/src/ConfigMessage.o: src/ConfigMessage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ConfigMessage.o src/ConfigMessage.cpp

${OBJECTDIR}/src/ConfigParser.o: src/ConfigParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ConfigParser.o src/ConfigParser.cpp

${OBJECTDIR}/src/MessageParser.o: src/MessageParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MessageParser.o src/MessageParser.cpp

${OBJECTDIR}/src/MessageRegister.o: src/MessageRegister.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MessageRegister.o src/MessageRegister.cpp

${OBJECTDIR}/src/ModuleData.o: src/ModuleData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ModuleData.o src/ModuleData.cpp

${OBJECTDIR}/src/Server.o: src/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Server.o src/Server.cpp

${OBJECTDIR}/src/ServerSession.o: src/ServerSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ServerSession.o src/ServerSession.cpp

${OBJECTDIR}/src/Task.o: src/Task.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Task.o src/Task.cpp

${OBJECTDIR}/src/TaskInstance.o: src/TaskInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TaskInstance.o src/TaskInstance.cpp

${OBJECTDIR}/src/TaskLoader.o: src/TaskLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TaskLoader.o src/TaskLoader.cpp

${OBJECTDIR}/src/TaskManager.o: src/TaskManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TaskManager.o src/TaskManager.cpp

${OBJECTDIR}/src/TimedTaskInstance.o: src/TimedTaskInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TimedTaskInstance.o src/TimedTaskInstance.cpp

${OBJECTDIR}/src/TriggerTaskInstance.o: src/TriggerTaskInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TriggerTaskInstance.o src/TriggerTaskInstance.cpp

${OBJECTDIR}/src/framework.o: src/framework.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/framework.o src/framework.cpp

${OBJECTDIR}/src/pugixml.o: src/pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pugixml.o src/pugixml.cpp

${OBJECTDIR}/src/u_server.o: src/u_server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/u_server.o src/u_server.cpp

${OBJECTDIR}/src/u_session.o: src/u_session.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/soci -I/usr/include/postgresql -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/u_session.o src/u_session.cpp

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
