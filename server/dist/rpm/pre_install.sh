#! /bin/sh

case "$1" in
1) # install
	/usr/sbin/groupadd -r beeeon
	/usr/bin/mkdir -p /var/lib/beeeon
	/usr/sbin/useradd -r -d /var/lib/beeeon/server-user -m -s /sbin/nologin -g beeeon beeeon_user
	/usr/sbin/useradd -r -d /var/lib/beeeon/server-admin -m -s /sbin/nologin -g beeeon beeeon_admin

	/usr/bin/mkdir -p /var/log/beeeon
	/usr/bin/chown root:beeeon /var/log/beeeon
	/usr/bin/chmod 0770 /var/log/beeeon
;;
2) # upgrade
;;
esac
