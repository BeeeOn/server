#!/bin/sh

DBname=$(cat DBname) 

echo "begin;
delete from users_adapters where fk_user_id = (select user_id from users where mail = '$1' );
delete from users where mail = '$1';
commit; " | psql $DBname
