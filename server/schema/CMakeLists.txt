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

set (SCHEMA_POSTGRE_ISQL_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/schema.postgre.isql.xsl"
	"${SCHEMA}.xml")

set (QUERIES_SQLITE_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/query.export.sqlite.xsl"
	"${SCHEMA}-query.xml")

set (QUERIES_POSTGRE_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/query.export.postgre.xsl"
	"${SCHEMA}-query.xml")

set (TEST_SQLITE_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/query.test.sqlite.xsl"
	"${SCHEMA}-query.xml")

set (TEST_POSTGRE_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/query.test.postgre.xsl"
	"${SCHEMA}-query.xml")

set (TEST_POSTGRE_ISQL_ARGS
	--stringparam codebase.version "${GIT_ID}"
	"${TOOLS_DIR}/xsl/query.test.postgre.isql.xsl"
	"${SCHEMA}-query.xml")

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

# create schema/${SCHEMA}-postgre-isql.sql
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${SCHEMA_POSTGRE_ISQL_ARGS})

# create schema/${SCHEMA}-sqlite-queries.ini
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${QUERIES_SQLITE_ARGS})

# create schema/${SCHEMA}-postgre-queries.ini
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${QUERIES_POSTGRE_ARGS})

# create schema/${SCHEMA}-sqlite-expect.csv and schema/${SCHEMA}-sqlite-test.sql
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${TEST_SQLITE_ARGS})

# create schema/${SCHEMA}-postgre-expect.csv and schema/${SCHEMA}-postgre-test.sql
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${TEST_POSTGRE_ARGS})

# create schema/${SCHEMA}-postgre-expect-isql.csv and schema/${SCHEMA}-postgre-test-isql.sql
exec_program(
	"${XSLTP}"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS ${TEST_POSTGRE_ISQL_ARGS})
