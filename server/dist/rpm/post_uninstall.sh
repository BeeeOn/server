#! /bin/sh

case "$1" in
0) # uninstall
	rmdir --ignore-fail-on-non-empty -v /var/log/beeeon

	echo "removing users beeeon_user, beeeon_admin..."
	/usr/sbin/userdel beeeon_admin
	/usr/sbin/userdel beeeon_user

	echo "removing group beeeon..."
	/usr/sbin/groupdel beeeon

	rmdir --ignore-fail-on-non-empty -v /var/lib/beeeon/server-admin
	rmdir --ignore-fail-on-non-empty -v /var/lib/beeeon/server-user
	rmdir --ignore-fail-on-non-empty -v /var/lib/beeeon
;;
1) # upgrade
;;
esac
