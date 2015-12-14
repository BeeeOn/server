#!/bin/sh

DBname=$(cat DBname) 

echo "select * from users_adapters;" | psql $DBname
