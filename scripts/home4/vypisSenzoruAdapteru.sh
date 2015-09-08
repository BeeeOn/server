#!/bin/sh
DBname=$(cat DBname) 

echo "select * from facilities join devices on fk_facilities_mac=mac where fk_adapter_id="$1 | psql $DBname
