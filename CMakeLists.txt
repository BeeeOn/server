cmake_minimum_required (VERSION 2.8.11)
project (server C CXX)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -pedantic -Wall -lpthread -ggdb3")
add_definitions(-std=c++11)

if(DEFINED ENV{GIT_ID})
	set (GIT_ID $ENV{GIT_ID})
else()
	exec_program(
		"git"
		${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "describe --always --tags"
		OUTPUT_VARIABLE GIT_ID)
endif()

exec_program(
	"genrb"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS
	-d res/beeeon_server
	res/beeeon_server/res_index.txt
	res/beeeon_server/root.txt
	res/beeeon_server/en.txt
	res/beeeon_server/cs.txt
)

if(NOT DEFINED LIB_INSTALL_DIR)
set(LIB_INSTALL_DIR "lib")
endif()

if(NOT DEFINED AR_INSTALL_DIR)
set(AR_INSTALL_DIR "lib")
endif()

# When installing into /usr, use /etc for configuration
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
set(SYSCONF_INSTALL_DIR "/etc")
else()
set(SYSCONF_INSTALL_DIR "etc")
endif()

add_subdirectory(src)
add_subdirectory(base)
add_subdirectory(test)

find_program(XSLTPROC NAMES xsltproc)
if(XSLTPROC)
	execute_process(COMMAND
		"${XSLTPROC}"
		"${CMAKE_CURRENT_SOURCE_DIR}/tools/devices_summary.xsl"
		"${CMAKE_CURRENT_SOURCE_DIR}/conf/devices.xml"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/supported_devices.md")
	set(SUPPORTED_DEVICES "INPUT += ${CMAKE_CURRENT_BINARY_DIR}/supported_devices.md")
endif()

find_package(Doxygen)

if(DOXYGEN_FOUND)
	set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT})
	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM)
endif()

find_program(MARKED_MAN NAMES marked-man)
find_program(RONN NAMES ronn)

if(MARKED_MAN)
	set(MD_MAN "${MARKED_MAN}")
	set(MD_MAN_ARGS "")
elseif(RONN)
	set(MD_MAN "${RONN}")
	set(MD_MAN_ARGS --roff --pipe)
endif()

if(MD_MAN)
	message(STATUS "generating man pages")
	file(GLOB MD_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/doc" ${CMAKE_CURRENT_SOURCE_DIR}/doc/*.md)
	file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man")

	foreach(md ${MD_FILES})
		message(STATUS "generating man from ${md}")
		string(REPLACE ".md" ".7" man "${md}")
		execute_process(COMMAND
			${MD_MAN} ${MD_MAN_ARGS} "${CMAKE_CURRENT_SOURCE_DIR}/doc/${md}"
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
			OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/man/beeeon-server-${man}")
	endforeach()

	install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man/"
		DESTINATION share/man/man7)
else()
	message(STATUS "missing tool for man pages generation, skipping")
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "2018")
set(CPACK_PACKAGE_VERSION_MINOR "7")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_NAME "beeeon-server")
set(CPACK_PACKAGE_VENDOR "Brno University of Technology")
set(CPACK_PACKAGE_CONTACT "Jan Viktorin <iviktorin@fit.vutbr.cz>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
	"BeeeOn Server - part of the BeeeOn IoT platform")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

set(CPACK_RPM_PACKAGE_RELEASE "0.rc0")
set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
set(CPACK_RPM_PACKAGE_GROUP "Other")
set(CPACK_RPM_PACKAGE_URL "https://github/BeeeOn/server")
set(CPACK_RPM_COMPRESSION_TYPE "gzip")
set(RPM_FILE_NAME_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

if(NOT CPACK_RPM_PACKAGE_RELEASE EQUAL "")
	set(RPM_FILE_NAME_VERSION "${RPM_FILE_NAME_VERSION}.${CPACK_RPM_PACKAGE_RELEASE}")
endif()

set(CPACK_RPM_FILE_NAME
	"${CPACK_PACKAGE_NAME}-${RPM_FILE_NAME_VERSION}.${CPACK_RPM_PACKAGE_ARCHITECTURE}.rpm")

set(CPACK_RPM_PACKAGE_REQUIRES "libPocoFoundation48 >= 1.7.7")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libPocoDataODBC48 >= 1.7.7")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libPocoJSON48 >= 1.7.7")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libPocoNet48 >= 1.7.7")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libPocoNetSSL48 >= 1.7.7")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libPocoXML48 >= 1.7.7")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libPocoCrypto48 >= 1.7.7")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, libicu >= 50.1")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, unixODBC >= 2.3.1")
set(CPACK_RPM_PACKAGE_REQUIRES "${CPACK_RPM_PACKAGE_REQUIRES}, postgresql-odbc")

# FIXME: Suggests sometimes does not work:
# CPackRPM:Debug: *** error: line 24: Unknown tag: Suggests: cppzmq >= 4.1
#set(CPACK_RPM_PACKAGE_SUGGESTS "libjwt >= 1.8")
#set(CPACK_RPM_PACKAGE_SUGGESTS "cppzmq >= 4.1")

list(APPEND CPACK_RPM_USER_FILELIST "%attr(0775,root,root) /etc/beeeon")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0755,root,beeeon) /etc/beeeon/server")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0750,root,beeeon) /etc/beeeon/server/config.d")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/config.d/*")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/*.ini")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/*.xml")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/apikeys.properties")

if(MD_MAN)
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0755,root,root) /usr/share/man")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0755,root,root) /usr/share/man/man7/*")
endif()

set(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/pre_install.sh")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/post_install.sh")
set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/pre_uninstall.sh")
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/post_uninstall.sh")

include(CPack)

install(FILES conf/server-startup.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES conf/restui-api.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES conf/apikeys.properties
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server
	PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ)
install(DIRECTORY conf/config.d
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server
	PATTERN factory-testing.xml EXCLUDE)
install(FILES conf/devices.xml conf/types.xml conf/enums.xml conf/bitmaps.xml
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES tools/log-analyze.awk
	DESTINATION share/beeeon/tools
	PERMISSIONS
		OWNER_READ OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE)
install(DIRECTORY db/
	DESTINATION share/beeeon/db
	FILES_MATCHING
	PATTERN "db/sqitch.conf"
	PATTERN "db/sqitch.plan"
	PATTERN "db/deploy/*.sql"
	PATTERN "db/deploy/*/*.sql"
	PATTERN "db/verify/*.sql"
	PATTERN "db/verify/*/*.sql"
	PATTERN "db/revert/*.sql"
	PATTERN "db/revert/*/*.sql"
	PATTERN "db/tools/*"
	PATTERN "db/pgsql/*.sql"
	PATTERN "db/pgsql/*/*.sql"
	PATTERN "db/scripts/*.sql"
	PATTERN "db/test" EXCLUDE
	PATTERN "db/templates" EXCLUDE)
install(DIRECTORY cert/
	DESTINATION share/beeeon/cert
	FILES_MATCHING PATTERN "*.pem")
install(FILES dtd/types.dtd
	DESTINATION share/beeeon/dtd)
install(DIRECTORY res/
	DESTINATION share/beeeon/res
	FILES_MATCHING
	PATTERN "res/beeeon_server/*.res")
install(DIRECTORY systemd
	DESTINATION lib
	FILE_PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ
		WORLD_READ
	DIRECTORY_PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE)
