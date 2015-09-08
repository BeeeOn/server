#!/bin/sh
DBname=$(cat DBname) 

echo "begin;
delete from logs;
delete from facilities;
delete from devices;
delete from rooms;
delete from users_adapters;
delete from users;
delete from adapters;
commit;" | psql $DBname
