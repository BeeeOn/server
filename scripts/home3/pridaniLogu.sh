#!/bin/bash
DBname=$(cat DBname) 

value=$1
range=$2
interval=$3

echo "start value = $value range = $range"

start=$(date --date "$4" +%s)
end=$(date --date "$5" +%s)

echo unix time- start: $start end: $end interval: $interval

for ((i = $start; i <= $end ; i+=$interval)); do
	#time=$(date -d @$i +'%Y-%m-%d %H:%M:%S')
    echo "insert into logs	(fk_facilities_mac, fk_devices_type, timestamp, value) values
		('$6', '$7', '$i',$value)" | psql $DBname

    let value=$value+$(( ( RANDOM % (2*$range) )  - $range +1 ))
done
