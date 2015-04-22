
create user root superuser;         
create user xvampo01 superuser;
create user rasovsky superuser;  
create user xblaho03 superuser;
create user xlamac00 superuser;


create user uiserver with password 'xyz';
  grant connect on database home6 to uiserver;
  grant select,insert,update,delete on users,users_adapters,adapters,facilities,devices,logs,rooms,mobile_devices to uiserver;
  grant select on sequence users_user_id_seq to uiserver;
  grant usage on sequence users_user_id_seq to uiserver;
  grant select on sequence rooms_room_id_seq to uiserver;
  grant usage on sequence rooms_room_id_seq to uiserver;

create user adaserver with password 'xyz';
  grant connect on database home6 to adaserver;   
  grant select,insert,update,delete on adapters,facilities,devices,logs to adaserver;

create user framework with password 'xyz';          
  grant connect on database home6 to framework;
  grant select,insert,update,delete on u_algorithms, users_algorithms, a_algorithms, alghorithms_adapters, algo_devices to framework;

create user gamification with password 'xyz';       
  grant connect on database home6 to gamification;
  grant select,insert,update,delete on achievements, users_achievements, adapters_achievements to gamification;




#hba_file+ 
#user local md5
#[pg_ctl reload or SIGUP to postmaster] to refresh access restrictions on psql

# alltables: users,users_adapters,adapters,facilities,devices,logs,rooms,mobile_devices,a_algorithms,algo_devices,u_algorithms,users_algorithms,algorithms_adapters to george;
