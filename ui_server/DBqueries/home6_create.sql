-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2015-04-20 11:33:20.968




-- tables
-- Table: a_algorithms
CREATE TABLE a_algorithms (
    algorithm_id int  NOT NULL,
    name varchar(50)  NOT NULL,
    CONSTRAINT a_algorithms_pk PRIMARY KEY (algorithm_id)
);



-- Table: achievements
CREATE TABLE achievements (
    achievement_id int  NOT NULL,
    parent_id int  NULL,
    category int  NOT NULL,
    points int  NOT NULL,
    progress_total int  NOT NULL,
    visibility boolean  NOT NULL,
    range boolean  NOT NULL,
    notification boolean  NOT NULL,
    CONSTRAINT achievements_pk PRIMARY KEY (achievement_id)
);



-- Table: adapters
CREATE TABLE adapters (
    adapter_id decimal(20,0)  NOT NULL,
    name varchar(50)  NULL,
    version int  NULL,
    socket int  NULL,
    timezone smallint  NOT NULL DEFAULT '1',
    logging_period int  NOT NULL DEFAULT 7,
    CONSTRAINT check_positive_adapter_id CHECK (adapter_id >= 0) NOT DEFERRABLE INITIALLY IMMEDIATE ,
    CONSTRAINT adapters_pk PRIMARY KEY (adapter_id)
);



-- Table: adapters_achievements
CREATE TABLE adapters_achievements (
    fk_achievement_id int  NOT NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    progress_current int  NOT NULL,
    date bigint  NULL,
    CONSTRAINT adapters_achievements_pk PRIMARY KEY (fk_achievement_id,fk_adapter_id)
);



-- Table: algo_devices
CREATE TABLE algo_devices (
    fk_users_algorithms_id int  NOT NULL,
    fk_user_id int  NOT NULL,
    fk_algorithm_id int  NOT NULL,
    fk_facilities_mac decimal(10)  NOT NULL,
    fk_devices_type smallint  NOT NULL,
    algo_devices_id serial  NOT NULL,
    CONSTRAINT algo_devices_pk PRIMARY KEY (fk_users_algorithms_id,fk_user_id,fk_algorithm_id,fk_facilities_mac,fk_devices_type,algo_devices_id)
);



-- Table: algorithms_adapters
CREATE TABLE algorithms_adapters (
    fk_adapter_id decimal(20,0)  NOT NULL,
    fk_algorithm_id int  NOT NULL,
    email_list text  NOT NULL,
    CONSTRAINT algorithms_adapters_pk PRIMARY KEY (fk_adapter_id,fk_algorithm_id,email_list)
);



-- Table: devices
CREATE TABLE devices (
    fk_facilities_mac decimal(10)  NOT NULL,
    type smallint  NOT NULL,
    name varchar(50)  NOT NULL DEFAULT '',
    value real  NOT NULL,
    visibility char(1)  NOT NULL DEFAULT '1',
    CONSTRAINT devices_pk PRIMARY KEY (fk_facilities_mac,type)
);



-- Table: facilities
CREATE TABLE facilities (
    mac decimal(10)  NOT NULL,
    refresh int  NOT NULL DEFAULT 10,
    battery int  NOT NULL,
    quality int  NOT NULL,
    init char(1)  NOT NULL DEFAULT '0',
    involved bigint  NOT NULL,
    timestamp bigint  NOT NULL,
    fk_room_id bigint  NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    fk_type_id int  NULL,
    CONSTRAINT check_positive_mac CHECK (mac >= 0) NOT DEFERRABLE INITIALLY IMMEDIATE ,
    CONSTRAINT facilities_pk PRIMARY KEY (mac)
);

CREATE INDEX facilities_idx_adapter_id on facilities (fk_adapter_id ASC);




-- Table: logs
CREATE TABLE logs (
    timestamp bigint  NOT NULL,
    value real  NOT NULL,
    fk_facilities_mac decimal(10)  NOT NULL,
    fk_devices_type smallint  NOT NULL,
    CONSTRAINT logs_pk PRIMARY KEY (timestamp,fk_facilities_mac,fk_devices_type)
);

CREATE INDEX logs_idx on logs (timestamp ASC,value ASC,fk_facilities_mac ASC,fk_devices_type ASC);




-- Table: mobile_devices
CREATE TABLE mobile_devices (
    token text  NOT NULL,
    mobile_id text  NOT NULL,
    type varchar(255)  NULL,
    locale varchar(10)  NULL,
    push_notification text  NULL,
    fk_user_id int  NOT NULL,
    CONSTRAINT mobile_devices_pk PRIMARY KEY (mobile_id,fk_user_id)
);

CREATE UNIQUE INDEX mobile_device_idx on mobile_devices (token ASC);




-- Table: notifications
CREATE TABLE notifications (
    text text  NOT NULL,
    "level" smallint  NOT NULL,
    message_id bigint  NOT NULL,
    timestamp bigint  NOT NULL,
    fk_user_id int  NOT NULL,
    read boolean  NOT NULL,
    CONSTRAINT notifications_pk PRIMARY KEY (message_id)
);

CREATE INDEX notifications_idx_user_id on notifications (fk_user_id ASC);




-- Table: rooms
CREATE TABLE rooms (
    room_id bigserial  NOT NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    type smallint  NOT NULL,
    name varchar(50)  NOT NULL DEFAULT '',
    CONSTRAINT rooms_pk PRIMARY KEY (room_id,fk_adapter_id)
);



-- Table: types
CREATE TABLE types (
    type_id serial  NOT NULL,
    mac_range numrange  NOT NULL,
    default_name text  NOT NULL,
    CONSTRAINT types_pk PRIMARY KEY (type_id)
);
CREATE EXTENSION btree_gist;
CREATE INDEX idx_mac_range ON facility_types USING gist (mac_range with &&);
alter table facility_types add constraint range_nonoverlap exclude using gist (mac_range with &&);



-- Table: u_algorithms
CREATE TABLE u_algorithms (
    algorithm_id int  NOT NULL,
    name varchar(50)  NOT NULL,
    CONSTRAINT u_algorithms_pk PRIMARY KEY (algorithm_id)
);



-- Table: users
CREATE TABLE users (
    user_id serial  NOT NULL,
    mail varchar(250)  NULL,
    signin_count int  NOT NULL DEFAULT 0,
    phone_locale varchar(10)  NULL DEFAULT 'cs',
    verified_email boolean  NULL,
    name varchar(250)  NULL,
    given_name varchar(250)  NULL,
    family_name varchar(250)  NULL,
    link varchar(250)  NULL,
    picture varchar(250)  NULL,
    gender varchar(10)  NULL,
    google_locale varchar(10)  NULL,
    google_id text  NULL,
    password text  NULL,
    CONSTRAINT users_pk PRIMARY KEY (user_id)
);
CREATE UNIQUE INDEX idx_user_mail ON users (mail);
CREATE UNIQUE INDEX idx_user_name ON users (name);
CREATE UNIQUE INDEX idx_user_google_id ON users (google_id);



-- Table: users_achievements
CREATE TABLE users_achievements (
    fk_achievement_id int  NOT NULL,
    fk_user_id int  NOT NULL,
    progress_current int  NOT NULL,
    date bigint  NULL,
    CONSTRAINT users_achievements_pk PRIMARY KEY (fk_achievement_id,fk_user_id)
);



-- Table: users_adapters
CREATE TABLE users_adapters (
    fk_user_id int  NOT NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    role varchar(15)  NOT NULL CHECK (role='guest' OR role='user' OR role='admin' OR role='superuser'),
    CONSTRAINT users_adapters_pk PRIMARY KEY (fk_user_id,fk_adapter_id)
);



-- Table: users_algorithms
CREATE TABLE users_algorithms (
    fk_user_id int  NOT NULL,
    fk_algorithm_id int  NOT NULL,
    users_algorithms_id serial  NOT NULL,
    parameters text  NOT NULL,
    data text  NOT NULL,
    name text  NOT NULL,
    fk_adapter_id decimal(20,0)  NOT NULL,
    state text  NOT NULL,
    CONSTRAINT users_algorithms_pk PRIMARY KEY (fk_user_id,fk_algorithm_id,users_algorithms_id)
);



-- Table: views
CREATE TABLE views (
    name varchar(50)  NOT NULL,
    fk_user_id int  NOT NULL,
    icon smallint  NOT NULL DEFAULT 0,
    CONSTRAINT views_pk PRIMARY KEY (name,fk_user_id)
);



-- Table: views_devices
CREATE TABLE views_devices (
    fk_view_name varchar(50)  NOT NULL,
    fk_user_id int  NOT NULL,
    fk_facilities_mac decimal(10)  NOT NULL,
    fk_devices_type smallint  NOT NULL,
    CONSTRAINT views_devices_pk PRIMARY KEY (fk_view_name,fk_user_id,fk_facilities_mac,fk_devices_type)
);







-- foreign keys
-- Reference:  achievement_detail_achievements (table: users_achievements)


ALTER TABLE users_achievements ADD CONSTRAINT achievement_detail_achievements 
    FOREIGN KEY (fk_achievement_id)
    REFERENCES achievements (achievement_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  achievement_detail_users (table: users_achievements)


ALTER TABLE users_achievements ADD CONSTRAINT achievement_detail_users 
    FOREIGN KEY (fk_user_id)
    REFERENCES users (user_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  achievements_achievements (table: achievements)


ALTER TABLE achievements ADD CONSTRAINT achievements_achievements 
    FOREIGN KEY (parent_id)
    REFERENCES achievements (achievement_id)
    ON DELETE  SET NULL 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  adapters_achievements_achievements (table: adapters_achievements)


ALTER TABLE adapters_achievements ADD CONSTRAINT adapters_achievements_achievements 
    FOREIGN KEY (fk_achievement_id)
    REFERENCES achievements (achievement_id)
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  adapters_achievements_adapters (table: adapters_achievements)


ALTER TABLE adapters_achievements ADD CONSTRAINT adapters_achievements_adapters 
    FOREIGN KEY (fk_adapter_id)
    REFERENCES adapters (adapter_id)
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

-- Reference:  algo_devices_devices (table: algo_devices)


ALTER TABLE algo_devices ADD CONSTRAINT algo_devices_devices 
    FOREIGN KEY (fk_facilities_mac,fk_devices_type)
    REFERENCES devices (fk_facilities_mac,type)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  algo_devices_users_algorithms (table: algo_devices)


ALTER TABLE algo_devices ADD CONSTRAINT algo_devices_users_algorithms 
    FOREIGN KEY (fk_user_id,fk_algorithm_id,fk_users_algorithms_id)
    REFERENCES users_algorithms (fk_user_id,fk_algorithm_id,users_algorithms_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  algorithms_adapters_a_algorithms (table: algorithms_adapters)


ALTER TABLE algorithms_adapters ADD CONSTRAINT algorithms_adapters_a_algorithms 
    FOREIGN KEY (fk_algorithm_id)
    REFERENCES a_algorithms (algorithm_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  algorithms_adapters_adapters (table: algorithms_adapters)


ALTER TABLE algorithms_adapters ADD CONSTRAINT algorithms_adapters_adapters 
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

-- Reference:  facilities_facility_types (table: facilities)


ALTER TABLE facilities ADD CONSTRAINT facilities_facility_types 
    FOREIGN KEY (fk_type_id)
    REFERENCES types (type_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  facilities_rooms (table: facilities)


ALTER TABLE facilities ADD CONSTRAINT facilities_rooms 
    FOREIGN KEY (fk_room_id,fk_adapter_id)
    REFERENCES rooms (room_id,fk_adapter_id)
    ON DELETE  SET NULL 
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

-- Reference:  users_algorithms_adapters (table: users_algorithms)


ALTER TABLE users_algorithms ADD CONSTRAINT users_algorithms_adapters 
    FOREIGN KEY (fk_adapter_id)
    REFERENCES adapters (adapter_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  users_algorithms_algorithms (table: users_algorithms)


ALTER TABLE users_algorithms ADD CONSTRAINT users_algorithms_algorithms 
    FOREIGN KEY (fk_algorithm_id)
    REFERENCES u_algorithms (algorithm_id)
    ON DELETE  CASCADE 
    NOT DEFERRABLE 
    INITIALLY IMMEDIATE 
;

-- Reference:  users_algorithms_users (table: users_algorithms)


ALTER TABLE users_algorithms ADD CONSTRAINT users_algorithms_users 
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






-- End of file.
