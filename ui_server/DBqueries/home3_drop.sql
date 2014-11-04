-- Created by Vertabelo (http://vertabelo.com)
-- Script type: drop
-- Scope: [tables, references, sequences, views, procedures]
-- Generated at Thu Oct 23 17:45:30 UTC 2014


DROP FUNCTION insert_room_trigger_function() cascade;
DROP FUNCTION unreg_adapter_after_delete_last_superuser() cascade;
DROP FUNCTION null_room_before_delete() cascade;






-- foreign keys
ALTER TABLE actions DROP CONSTRAINT Actions_users;

ALTER TABLE conditions DROP CONSTRAINT Conditions_Actions;

ALTER TABLE conditions DROP CONSTRAINT Conditions_users;

ALTER TABLE users_adapters DROP CONSTRAINT adapters_users_adapters;

ALTER TABLE devices DROP CONSTRAINT devices_facilities;

ALTER TABLE facilities DROP CONSTRAINT facilities_adapters;

ALTER TABLE facilities DROP CONSTRAINT facilities_rooms;

ALTER TABLE gcm_ids DROP CONSTRAINT gcm_ids_users;

ALTER TABLE logs DROP CONSTRAINT logs_devices;

ALTER TABLE notifications DROP CONSTRAINT notifications_users;

ALTER TABLE rooms DROP CONSTRAINT rooms_adapters;

ALTER TABLE users_adapters DROP CONSTRAINT users_adapters_users;

ALTER TABLE views_devices DROP CONSTRAINT views_devices_devices;

ALTER TABLE views DROP CONSTRAINT views_users;

ALTER TABLE views_devices DROP CONSTRAINT views_views_devices;





-- tables
DROP TABLE actions;
DROP TABLE adapters;
DROP TABLE conditions;
DROP TABLE devices;
DROP TABLE facilities;
DROP TABLE gcm_ids;
DROP TABLE logs;
DROP TABLE notifications;
DROP TABLE rooms;
DROP TABLE users;
DROP TABLE users_adapters;
DROP TABLE views;
DROP TABLE views_devices;




-- End of file.

