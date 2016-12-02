cmake_minimum_required (VERSION 2.8.11)
project (schema)

set (TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tools)
set (SCHEMA home8)
set (XSLTP xsltproc)

set (SCHEMA_SQLITE_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/schema.sqlite.xsl"
	"${SCHEMA}.xml")

set (SCHEMA_POSTGRE_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/schema.postgre.xsl"
	"${SCHEMA}.xml")

# create schema/${SCHEMA}-sqlite.sql
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${SCHEMA_SQLITE_ARGS})

# create schema/${SCHEMA}-postgre.sql
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${SCHEMA_POSTGRE_ARGS})
