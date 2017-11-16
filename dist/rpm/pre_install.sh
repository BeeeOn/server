#! /bin/sh

case "$1" in
1) # install
	echo "creating group beeeon..."
	/usr/sbin/groupadd -r beeeon

	echo "creating users beeeon_user, beeeon_admin..."
	/usr/bin/mkdir -p /var/lib/beeeon
	/usr/sbin/useradd -r -d /var/lib/beeeon/server-user -m -s /sbin/nologin -g beeeon beeeon_user
	/usr/sbin/useradd -r -d /var/lib/beeeon/server-admin -m -s /sbin/nologin -g beeeon beeeon_admin

	/usr/bin/mkdir -p /var/log/beeeon
	/usr/bin/chown root:beeeon /var/log/beeeon
	/usr/bin/chmod 0770 /var/log/beeeon
;;
2) # upgrade
	echo "stopping service beeeon-server..."
	/usr/bin/systemctl stop beeeon-server.service \
		&& echo "service beeeon-server has been stopped" \
		|| echo "failed to stop beeeon-server service"
;;
esac
