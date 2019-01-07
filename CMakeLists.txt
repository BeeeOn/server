cmake_minimum_required (VERSION 2.8.11)
project (server C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -pedantic -Wall -lpthread -ggdb3")
add_definitions(-std=c++11)

include(GitSupport)

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

set(REST_API_DOC "INPUT += ${CMAKE_CURRENT_BINARY_DIR}/restui.h")
set(GWS_API_DOC "INPUT += ${CMAKE_CURRENT_BINARY_DIR}/gws.h")

add_custom_target(restapi-doc
	COMMAND
		"${CMAKE_CURRENT_SOURCE_DIR}/tools/restapi-doc.sh"
		"${CMAKE_CURRENT_SOURCE_DIR}"
		> "${CMAKE_CURRENT_BINARY_DIR}/restui.h"
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(gws-doc
	COMMAND
		"${CMAKE_CURRENT_SOURCE_DIR}/tools/gws-doc.sh"
		"${CMAKE_CURRENT_SOURCE_DIR}"
		> "${CMAKE_CURRENT_BINARY_DIR}/gws.h"
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

include(DevicesSummary)
include(DoxygenSupport)
add_dependencies(doc restapi-doc)
add_dependencies(doc gws-doc)
include(ManSupport)

set(CPACK_PACKAGE_VERSION_MAJOR "2018")
set(CPACK_PACKAGE_VERSION_MINOR "11")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_NAME "beeeon-server")
set(CPACK_PACKAGE_VENDOR "Brno University of Technology")
set(CPACK_PACKAGE_CONTACT "Jan Viktorin <iviktorin@fit.vutbr.cz>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
	"BeeeOn Server - part of the BeeeOn IoT platform")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

set(CPACK_RPM_PACKAGE_RELEASE "0.rc5")
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

set(CPACK_RPM_CORE_PACKAGE_REQUIRES "libPocoFoundation48 >= 1.7.7")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, libPocoDataODBC48 >= 1.7.7")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, libPocoJSON48 >= 1.7.7")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, libPocoNet48 >= 1.7.7")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, libPocoNetSSL48 >= 1.7.7")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, libPocoXML48 >= 1.7.7")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, libPocoCrypto48 >= 1.7.7")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, libicu >= 50.1")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, unixODBC >= 2.3.1")
set(CPACK_RPM_CORE_PACKAGE_REQUIRES "${CPACK_RPM_CORE_PACKAGE_REQUIRES}, postgresql-odbc")

# FIXME: Suggests sometimes does not work:
# CPackRPM:Debug: *** error: line 24: Unknown tag: Suggests: cppzmq >= 4.1
#set(CPACK_RPM_PACKAGE_SUGGESTS "libjwt >= 1.8")
#set(CPACK_RPM_PACKAGE_SUGGESTS "cppzmq >= 4.1")

list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0775,root,root) /etc/beeeon")
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0755,root,beeeon) /etc/beeeon/server")
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0750,root,beeeon) /etc/beeeon/server/config.d")
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/config.d/*")
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/*.ini")
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/*.xml")
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0640,root,beeeon) %config /etc/beeeon/server/apikeys.properties")

if(MD_MAN)
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0755,root,root) /usr/share/man")
list(APPEND CPACK_RPM_CORE_USER_FILELIST "%attr(0755,root,root) /usr/share/man/man7/*")
endif()

set(CPACK_RPM_CORE_PRE_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/pre_install.sh")
set(CPACK_RPM_CORE_POST_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/post_install.sh")
set(CPACK_RPM_CORE_PRE_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/pre_uninstall.sh")
set(CPACK_RPM_CORE_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/rpm/post_uninstall.sh")

set(CPACK_COMPONENTS_ALL core cassandra)
set(CPACK_RPM_MAIN_COMPONENT core)

set(CPACK_COMPONENT_CORE_DISPLAY_NAME "BeeeOn Server")

set(CPACK_COMPONENT_CASSANDRA_DEPENDS core)
set(CPACK_COMPONENT_CASSANDRA_DISPLAY_NAME "BeeeOn Server - support for Cassandra")
set(CPACK_RPM_CASSANDRA_PACKAGE_SUMMARY "${CPACK_COMPONENT_CASSANDRA_DISPLAY_NAME}")
set(CPACK_RPM_CASSANDRA_FILE_NAME
	"${CPACK_PACKAGE_NAME}-cassandra-${RPM_FILE_NAME_VERSION}.${CPACK_RPM_PACKAGE_ARCHITECTURE}.rpm")
set(CPACK_RPM_CASSANDRA_PACKAGE_REQUIRES "beeeon-server >= ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_RPM_CASSANDRA_PACKAGE_REQUIRES "${CPACK_RPM_CASSANDRA_PACKAGE_REQUIRES}, libpocodatacassandra50")

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_RPM_COMPONENT_INSTALL ON)

include(CPack)

install(FILES conf/server-startup.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server
	COMPONENT core)
install(FILES conf/restui-api.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server
	COMPONENT core)
install(FILES conf/apikeys.properties
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server
	PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ
	COMPONENT core)
install(DIRECTORY conf/config.d
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server
	COMPONENT core
	PATTERN factory-testing.xml EXCLUDE)
install(FILES conf/devices.xml conf/types.xml conf/enums.xml conf/bitmaps.xml
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server
	COMPONENT core)
install(FILES tools/log-analyze.awk
	DESTINATION share/beeeon/tools
	PERMISSIONS
		OWNER_READ OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
	COMPONENT core)
install(FILES db/tools/db-admin.sh
	DESTINATION bin
	RENAME beeeon-db-admin
	PERMISSIONS
		OWNER_READ OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
	COMPONENT core)
install(DIRECTORY db/
	DESTINATION share/beeeon/db
	COMPONENT core
	FILES_MATCHING
	PATTERN "db/pgsql/sqitch.conf"
	PATTERN "db/pgsql/sqitch.plan"
	PATTERN "db/pgsql/deploy/*.sql"
	PATTERN "db/pgsql/deploy/*/*.sql"
	PATTERN "db/pgsql/verify/*.sql"
	PATTERN "db/pgsql/verify/*/*.sql"
	PATTERN "db/pgsql/revert/*.sql"
	PATTERN "db/pgsql/revert/*/*.sql"
	PATTERN "db/tools/*"
	PATTERN "db/pgsql/queries/*.sql"
	PATTERN "db/pgsql/queries/*/*.sql"
	PATTERN "db/scripts/*.sql"
	PATTERN "db/pgsql/test" EXCLUDE
	PATTERN "db/pgsql/templates" EXCLUDE)
install(DIRECTORY cert/
	DESTINATION share/beeeon/cert
	COMPONENT core
	FILES_MATCHING PATTERN "*.pem")
install(FILES dtd/types.dtd
	DESTINATION share/beeeon/dtd
	COMPONENT core)
install(DIRECTORY res/
	DESTINATION share/beeeon/res
	COMPONENT core
	FILES_MATCHING
	PATTERN "res/beeeon_server/*.res")
install(DIRECTORY dist/systemd
	DESTINATION lib
	FILE_PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ
		WORLD_READ
	DIRECTORY_PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
	COMPONENT core)
