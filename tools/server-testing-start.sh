#! /bin/sh

test -z "${BIN_DIR}"  && BIN_DIR=build_x86/src
test -z "${DB_DIR}"   && DB_DIR=db
test -z "${CONF_DIR}" && CONF_DIR=conf
test -z "${TARGET}"   && TARGET=xmlui
test -z "${TIMEOUT}"  && TIMEOUT=-1
test -z "${STARTUP}"  && STARTUP="${CONF_DIR}/testing-startup.ini"

BEEEON_SERVER=${BIN_DIR}/beeeon-server
BEEEON_CONF="${STARTUP}"

die()
{
	echo "$@" >&2
	exit 1
}

die_and_kill()
{
	kill ${1}
	shift
	die "$@"
}

postgres_start()
{
	${DB_DIR}/tools/postgresql.py || die "failed to start postgresql"
}

server_start()
{
	local host
	local port
	local user
	local name
	local pid

	read -r host || die "failed to read database host"
	read -r port || die "failed to read database port"
	read -r user || die "failed to read database user"
	read -r name || die "failed to read database name"
	read -r pid  || die "failed to read database pid"

	if [ "${TIMEOUT}" -le 0 ]; then
		server_start="${BEEEON_SERVER}"
	else
		server_start="timeout --preserve-status ${TIMEOUT}s ${BEEEON_SERVER}"
	fi

	${server_start} -c ${BEEEON_CONF} -Dapplication.di.runner=${TARGET} \
		-Ddatabase.host=${host}    \
		-Ddatabase.port=${port}    \
		-Ddatabase.user=${user}    \
		-Ddatabase.name=${name}    \
		"$@"                       \
		|| die_and_kill ${pid} "beeeon-server has failed"

	kill ${pid}
}

postgres_start | server_start "$@"
