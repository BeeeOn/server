cmake_minimum_required (VERSION 2.8.11)
project (server C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/modules)

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

add_definitions(-DGIT_ID="${GIT_ID}")

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

set(CPACK_PACKAGE_VERSION_MAJOR "2017")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_NAME "beeeon-server")
set(CPACK_PACKAGE_VENDOR "Brno University of Technology")
set(CPACK_PACKAGE_CONTACT "Jan Viktorin <iviktorin@fit.vutbr.cz>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
	"BeeeOn Server - part of the BeeeOn IoT platform")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.md)

set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
set(CPACK_RPM_PACKAGE_GROUP "Other")
set(CPACK_RPM_PACKAGE_URL "https://github/BeeeOn/server")
set(CPACK_RPM_COMPRESSION_TYPE "gzip")
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
set(CPACK_RPM_PACKAGE_SUGGESTS "libjwt >= 1.8")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0750,root,beeeon) /etc/beeeon/db")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0755,root,beeeon) /etc/beeeon/server")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0750,root,beeeon) /etc/beeeon/server/config.d")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0640,root,beeeon) /etc/beeeon/server/config.d/*")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0640,root,beeeon) /etc/beeeon/server/*.ini")
list(APPEND CPACK_RPM_USER_FILELIST "%attr(0640,root,beeeon) /etc/beeeon/server/*.xml")

include(CPack)

install(FILES conf/server-startup.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES conf/restui-api.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(DIRECTORY conf/config.d
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES conf/devices.xml conf/types.xml
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES db/server-queries.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/db)
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
