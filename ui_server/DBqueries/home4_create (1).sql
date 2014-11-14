-- Created by Vertabelo (http://vertabelo.com)
-- Script type: create
-- Scope: [tables, references, sequences, views, procedures]
-- Generated at Fri Nov 14 15:07:32 UTC 2014




-- tables
-- Table: actions
CREATE TABLE actions (
    action_id serial  NOT NULL,
    name varchar(25)  NOT NULL,
    xml xml  NOT NULL,
    fk_user_id decimal(21,0)  NOT NULL,
    CONSTRAINT actions_pk PRIMARY KEY (action_id)
);



-- Table: adapters
CREATE TABLE adapters (
    adapter_id decimal(20,0)  NOT NULL,
    name varchar(50)  NULL,
    version int  NULL,
    ip_address inet  NULL,
    registrable char(1)  NOT NULL DEFAULT '1',
    timezone smallint  NOT NULL DEFAULT '1',
    logging_period int  NOT NULL DEFAULT 7,
    CONSTRAINT adapters_pk PRIMARY KEY (adapter_id)
);



-- Table: conditions
CREATE TABLE conditions (
    cond_id serial  NOT NULL,
    name varchar(25)  NOT NULL,
    type varchar(3)  NOT NULL,
    xml xml  NOT NULL,
    fk_user_id decimal(21,0)  NOT NULL,
    fk_action_id int  NULL,
    CONSTRAINT conditions_pk PRIMARY KEY (cond_id)
);



-- Table: devices
CREATE TABLE devices (
    fk_facilities_mac inet  NOT NULL,
    type smallint  NOT NULL,
    name varchar(50)  NOT NULL DEFAULT '',
    value varchar(10)  NOT NULL,
    visibility char(1)  NOT NULL DEFAULT '1',
    CONSTRAINT devices_pk PRIMARY KEY (fk_facilities_mac,type)
);



-- Table: facilities
CREATE TABLE facilities (
    mac inet  NOT NULL,
    refresh int  NOT NULL DEFAULT 10,
    battery int  NOT NULL,
    quality int  NOT NULL,
    init char(1)  NOT NULL DEFAULT '0',
    involved bigint  NOT NULL,
    timestamp bigint  NOT NULL,
    fk_room_id bigint  NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    CONSTRAINT facilities_pk PRIMARY KEY (mac)
);



-- Table: gcm_ids
CREATE TABLE gcm_ids (
    gcm_id text  NOT NULL,
    fk_user_id decimal(21,0)  NOT NULL,
    CONSTRAINT gcm_ids_pk PRIMARY KEY (gcm_id,fk_user_id)
);



-- Table: logs
CREATE TABLE logs (
    timestamp bigint  NOT NULL,
    value real  NOT NULL,
    fk_facilities_mac inet  NOT NULL,
    fk_devices_type smallint  NOT NULL,
    CONSTRAINT logs_pk PRIMARY KEY (timestamp,fk_facilities_mac,fk_devices_type)
);



-- Table: mobile_devices
CREATE TABLE mobile_devices (
    token bigint  NOT NULL,
    fk_user_id decimal(21,0)  NOT NULL,
    type varchar(255)  NULL,
    ip inet  NULL,
    CONSTRAINT mobile_devices_pk PRIMARY KEY (token)
);



-- Table: notifications
CREATE TABLE notifications (
    text text  NOT NULL,
    "level" smallint  NOT NULL,
    message_id bigint  NOT NULL,
    timestamp timestamp  NOT NULL,
    fk_user_id decimal(21,0)  NOT NULL,
    CONSTRAINT notifications_pk PRIMARY KEY (message_id)
);



-- Table: rooms
CREATE TABLE rooms (
    room_id bigserial  NOT NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    type smallint  NOT NULL,
    name varchar(50)  NOT NULL DEFAULT '',
    CONSTRAINT rooms_pk PRIMARY KEY (room_id)
);



-- Table: users
CREATE TABLE users (
    user_id decimal(21,0)  NOT NULL,
    mail varchar(250)  NOT NULL,
    signin_count int  NOT NULL DEFAULT 0,
    phone_locale varchar(10)  NOT NULL DEFAULT 'cs',
    google_id varchar(250)  NULL,
    verified_email boolean  NULL,
    name varchar(250)  NULL,
    given_name varchar(250)  NULL,
    family_name varchar(250)  NULL,
    link varchar(250)  NULL,
    picture varchar(250)  NULL,
    gender varchar(10)  NULL,
    google_locale varchar(10)  NULL,
    CONSTRAINT users_pk PRIMARY KEY (user_id)
);
ALTER TABLE users ADD UNIQUE (mail);




-- Table: users_adapters
CREATE TABLE users_adapters (
    fk_user_id decimal(21,0)  NOT NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    role varchar(15)  NOT NULL CHECK (role='guest' OR role='user' OR role='admin' OR role='superuser'),
    CONSTRAINT users_adapters_pk PRIMARY KEY (fk_user_id,fk_adapter_id)
);



-- Table: views
CREATE TABLE views (
    name varchar(50)  NOT NULL,
    fk_user_id decimal(21,0)  NOT NULL,
    icon smallint  NOT NULL DEFAULT 0,
    CONSTRAINT views_pk PRIMARY KEY (name,fk_user_id)
);



-- Table: views_devices
CREATE TABLE views_devices (
    fk_view_name varchar(50)  NOT NULL,
    fk_user_id decimal(21,0)  NOT NULL,
    fk_facilities_mac inet  NOT NULL,
    fk_devices_type smallint  NOT NULL,
    CONSTRAINT views_devices_pk PRIMARY KEY (fk_view_name,fk_user_id,fk_facilities_mac,fk_devices_type)
);







-- foreign keys
-- Reference:  Actions_users (table: actions)


ALTER TABLE actions ADD CONSTRAINT Actions_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  Conditions_Actions (table: conditions)


ALTER TABLE conditions ADD CONSTRAINT Conditions_Actions 
    FOREIGN KEY (fk_action_id)
    REFERENCES actions (action_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  Conditions_users (table: conditions)


ALTER TABLE conditions ADD CONSTRAINT Conditions_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  adapters_users_adapters (table: users_adapters)


ALTER TABLE users_adapters ADD CONSTRAINT adapters_users_adapters 
    FOREIGN KEY (fk_adapter_id)
    REFERENCES adapters (adapter_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  devices_facilities (table: devices)


ALTER TABLE devices ADD CONSTRAINT devices_facilities 
    FOREIGN KEY (fk_facilities_mac)
    REFERENCES facilities (mac)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  facilities_adapters (table: facilities)


ALTER TABLE facilities ADD CONSTRAINT facilities_adapters 
    FOREIGN KEY (fk_adapter_id)
    REFERENCES adapters (adapter_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  facilities_rooms (table: facilities)


ALTER TABLE facilities ADD CONSTRAINT facilities_rooms 
    FOREIGN KEY (fk_room_id)
    REFERENCES rooms (room_id)
    ON DELETE  SET NULL 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  gcm_ids_users (table: gcm_ids)


ALTER TABLE gcm_ids ADD CONSTRAINT gcm_ids_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  logs_devices (table: logs)


ALTER TABLE logs ADD CONSTRAINT logs_devices 
    FOREIGN KEY (fk_facilities_mac,fk_devices_type)
    REFERENCES devices (fk_facilities_mac,type)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  mobile_devices_users (table: mobile_devices)


ALTER TABLE mobile_devices ADD CONSTRAINT mobile_devices_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  notifications_users (table: notifications)


ALTER TABLE notifications ADD CONSTRAINT notifications_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  rooms_adapters (table: rooms)


ALTER TABLE rooms ADD CONSTRAINT rooms_adapters 
    FOREIGN KEY (fk_adapter_id)
    REFERENCES adapters (adapter_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  users_adapters_users (table: users_adapters)


ALTER TABLE users_adapters ADD CONSTRAINT users_adapters_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  views_devices_devices (table: views_devices)


ALTER TABLE views_devices ADD CONSTRAINT views_devices_devices 
    FOREIGN KEY (fk_facilities_mac,fk_devices_type)
    REFERENCES devices (fk_facilities_mac,type)
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  views_users (table: views)


ALTER TABLE views ADD CONSTRAINT views_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  views_views_devices (table: views_devices)


ALTER TABLE views_devices ADD CONSTRAINT views_views_devices 
    FOREIGN KEY (fk_view_name,fk_user_id)
    REFERENCES views (name,fk_user_id)
    ON DELETE  CASCADE 
    ON UPDATE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;






CREATE OR REPLACE FUNCTION check_downgrade_last_superuser(adapter_id int, user_id int)
RETURNS BOOLEAN AS $$
BEGIN
IF
        (SELECT COUNT(*)
        FROM users_adapters 
        WHERE fk_adapter_id=$1 and role='superuser' ) = 1 AND
        (SELECT COUNT(*)
        FROM users_adapters 
        WHERE fk_adapter_id=$1 and role='superuser' and fk_user_id=$2) = 1 THEN
        RETURN false;
ELSE        
        RETURN true;
END IF;
END;
$$  LANGUAGE plpgsql ;


CREATE OR REPLACE FUNCTION unreg_adapter_after_delete_last_superuser()
RETURNS trigger AS $unreg_adapter_after_delete_last_superuser$
DECLARE
BEGIN
IF ( SELECT COUNT(*) FROM users_adapters WHERE fk_adapter_id=old.fk_adapter_id and role='superuser' ) = 1  AND
    old.role = 'superuser'    
THEN 
    update adapters set registrable=1, name='' where adapter_id=old.fk_adapter_id;
END IF;
RETURN old;
END;
$unreg_adapter_after_delete_last_superuser$  LANGUAGE plpgsql ;

CREATE TRIGGER unreg_adapter_after_delete_last_superuser
    BEFORE DELETE ON users_adapters
        FOR EACH ROW EXECUTE PROCEDURE unreg_adapter_after_delete_last_superuser();



-- End of file.

