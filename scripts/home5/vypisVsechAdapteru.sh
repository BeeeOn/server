#!/bin/sh
DBname=$(cat DBname) 

echo "select * from adapters"| psql $DBname
