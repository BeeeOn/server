#!/bin/sh
DBname=$(cat DBname) 

echo "delete from facilities where mac='$1'" | psql $DBname
