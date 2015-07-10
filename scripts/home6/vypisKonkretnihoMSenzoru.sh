#!/bin/sh
DBname=$(cat DBname) 


echo "select * from devices join facilities on mac= fk_facilities_mac where mac='"$1"'" | psql $DBname
