-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2015-05-03 18:51:26.473






-- foreign keys
ALTER TABLE users_achievements DROP CONSTRAINT achievement_detail_achievements;

ALTER TABLE users_achievements DROP CONSTRAINT achievement_detail_users;

ALTER TABLE achievements DROP CONSTRAINT achievements_achievements;

ALTER TABLE adapters_achievements DROP CONSTRAINT adapters_achievements_achievements;

ALTER TABLE adapters_achievements DROP CONSTRAINT adapters_achievements_adapters;

ALTER TABLE users_adapters DROP CONSTRAINT adapters_users_adapters;

ALTER TABLE algo_devices DROP CONSTRAINT algo_devices_devices;

ALTER TABLE algo_devices DROP CONSTRAINT algo_devices_users_algorithms;

ALTER TABLE algorithms_adapters DROP CONSTRAINT algorithms_adapters_adapters;

ALTER TABLE algorithms_adapters DROP CONSTRAINT algorithms_adapters_u_algorithms;

ALTER TABLE devices DROP CONSTRAINT devices_facilities;

ALTER TABLE facilities DROP CONSTRAINT facilities_adapters;

ALTER TABLE facilities DROP CONSTRAINT facilities_rooms;

ALTER TABLE logs DROP CONSTRAINT logs_devices;

ALTER TABLE mobile_devices DROP CONSTRAINT mobile_devices_users;

ALTER TABLE notifications DROP CONSTRAINT notifications_users;

ALTER TABLE rooms DROP CONSTRAINT rooms_adapters;

ALTER TABLE users_adapters DROP CONSTRAINT users_adapters_users;

ALTER TABLE users_algorithms DROP CONSTRAINT users_algorithms_adapters;

ALTER TABLE users_algorithms DROP CONSTRAINT users_algorithms_algorithms;

ALTER TABLE users_algorithms DROP CONSTRAINT users_algorithms_users;

ALTER TABLE views_devices DROP CONSTRAINT views_devices_devices;

ALTER TABLE views DROP CONSTRAINT views_users;

ALTER TABLE views_devices DROP CONSTRAINT views_views_devices;





-- tables
DROP TABLE achievements;
DROP TABLE adapters;
DROP TABLE adapters_achievements;
DROP TABLE algo_devices;
DROP TABLE algorithms_adapters;
DROP TABLE devices;
DROP TABLE facilities;
DROP TABLE logs;
DROP TABLE mobile_devices;
DROP TABLE notifications;
DROP TABLE rooms;
DROP TABLE types;
DROP TABLE u_algorithms;
DROP TABLE users;
DROP TABLE users_achievements;
DROP TABLE users_adapters;
DROP TABLE users_algorithms;
DROP TABLE views;
DROP TABLE views_devices;




-- End of file.

