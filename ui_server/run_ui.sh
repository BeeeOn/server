mv log ./logs/$(date +log-%y%m%d_%H%M%S)
ulimit -c unlimited

port=4566

portstatus=$( netstat -an | grep $port )

while  [ -n "$portstatus" ]; do
    echo "port is occupied"
    sleep 1
    portstatus=$( netstat -an | grep $port )
done
echo "starting ui_server"
./ui_server config_devel.xml >> log 2>&1