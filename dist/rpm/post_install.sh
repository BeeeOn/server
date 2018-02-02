#! /bin/sh

list_systemd_deps()
{
	pgsql_count=`systemctl list-unit-files --plain | grep postgresql | grep -v '^postgresql\.service' | wc -l`

	if [ "${pgsql_count}" > 0 ]; then
		echo "found non-standard postgresql services:"
		systemctl list-unit-files | awk '
			/^postgresql/ && $2 == "enabled" {printf "  %s", $1}
			/^postgresql/ && $2 != "enabled" {printf "  %s [%s]", $1, $2}
			$1 == "postgresql.service" {printf " (*)"}
			/^postgresql/ {print ""; next}
		'
		echo ""
		echo "  you can change the dependency on postgresql systemd service"
		echo "  during installation by setting the environment variable"
		echo "  BEEEON_DEP_PGSQL (sudo might require -E option)"
		echo ""
	fi
}

fixup_systemd_deps()
{
	if [ -n "${BEEEON_DEPS_PGSQL}" ]; then
		if ! grep '/\.service$/' <<< "${BEEEON_DEPS_PGSQL}" >/dev/null 2>/dev/null; then
			BEEEON_DEPS_PGSQL="${BEEEON_DEPS_PGSQL}.service"
		fi

		echo "fixup systemd postgresql service to '${BEEEON_DEPS_PGSQL}'"

		sed -i "s/=postgresql\.service/=${BEEEON_DEPS_PGSQL}/" \
			/usr/lib/systemd/system/beeeon-server.service

		echo ""
	fi
}

case "$1" in
1) # install
	list_systemd_deps
	fixup_systemd_deps
;;
2) # upgrade
	list_systemd_deps
	fixup_systemd_deps
;;
esac

echo "please, start beeeon-server service manually"
echo ""
echo "   systemctl start beeeon-server.service"
echo ""
