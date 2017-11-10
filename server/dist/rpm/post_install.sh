#! /bin/sh

case "$1" in
1) # install
;;
2) # upgrade
;;
esac

echo "please, start beeeon-server service manually"
echo ""
echo "   systemctl start beeeon-server.service"
echo ""
