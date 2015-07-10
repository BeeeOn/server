#!/bin/sh
DBname=$(cat DBname) 

echo "delete from devices where fk_facilities_mac='"$1"' and type="$2 | psql $DBname
