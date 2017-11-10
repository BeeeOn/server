#! /bin/sh

case "$1" in
0) # uninstall
	echo "stopping service beeeon-server..."
	/usr/bin/systemctl stop beeeon-server.service \
		&& echo "service beeeon-server has been stopped" \
		|| echo "failed to stop beeeon-server service"
;;
1) # upgrade
;;
esac
