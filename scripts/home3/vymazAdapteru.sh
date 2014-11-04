#!/bin/sh
DBname=$(cat DBname) 

echo "delete from adapters where adapter_id="$1 | psql $DBname
