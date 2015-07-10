#!/bin/sh
DBname=$(cat DBname) 

echo "INSERT INTO ADAPTERS (ADAPTER_ID,NAME,ip_address) VALUES ($1, 'adapter $1', '::$1')" | psql $DBname
