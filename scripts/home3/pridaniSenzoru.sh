#!/bin/sh
DBname=$(cat DBname) 

echo "INSERT INTO DEVICES	(fk_facilities_mac, type, name, value ) VALUES
		('$1','$2','senzor $1$2',$3)" | psql $DBname
