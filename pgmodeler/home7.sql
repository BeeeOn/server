-- Database generated with pgModeler (PostgreSQL Database Modeler).
-- pgModeler  version: 0.8.1-beta1
-- PostgreSQL version: 9.4
-- Project Site: pgmodeler.com.br
-- Model Author: ---

-- object: hardware | type: ROLE --
-- DROP ROLE IF EXISTS hardware;
CREATE ROLE hardware WITH ;
-- ddl-end --

-- object: user_data | type: ROLE --
-- DROP ROLE IF EXISTS user_data;
CREATE ROLE user_data WITH ;
-- ddl-end --

-- object: uiserver7 | type: ROLE --
-- DROP ROLE IF EXISTS uiserver7;
CREATE ROLE uiserver7 WITH 
	LOGIN
	UNENCRYPTED PASSWORD '1234'
	IN ROLE hardware,user_data;
-- ddl-end --

-- Appended SQL commands --
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public to uiserver7;
-- ddl-end --

-- object: algorithms | type: ROLE --
-- DROP ROLE IF EXISTS algorithms;
CREATE ROLE algorithms WITH ;
-- ddl-end --

-- object: framework7 | type: ROLE --
-- DROP ROLE IF EXISTS framework7;
CREATE ROLE framework7 WITH 
	IN ROLE algorithms,hardware,user_data;
-- ddl-end --

-- Appended SQL commands --
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public to framework7;
-- ddl-end --

-- object: adaserver7 | type: ROLE --
-- DROP ROLE IF EXISTS adaserver7;
CREATE ROLE adaserver7 WITH 
	IN ROLE hardware,user_data;
-- ddl-end --

-- Appended SQL commands --
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public to adaserver7;
-- ddl-end --


-- Database creation must be done outside an multicommand file.
-- These commands were put in this file only for convenience.
-- -- object: home7 | type: DATABASE --
-- -- DROP DATABASE IF EXISTS home7;
-- CREATE DATABASE home7
-- ;
-- -- ddl-end --
-- 

-- object: public.users | type: TABLE --
-- DROP TABLE IF EXISTS public.users CASCADE;
CREATE TABLE public.users(
	user_id serial NOT NULL,
	signin_count integer NOT NULL DEFAULT 0,
	given_name varchar(250),
	family_name varchar(250),
	picture varchar(250),
	gender varchar(10),
	mail varchar(250),
	password text,
	google_id text,
	facebook_id text,
	verified bool NOT NULL DEFAULT true,
	CONSTRAINT users_pk PRIMARY KEY (user_id)

);
-- ddl-end --
ALTER TABLE public.users OWNER TO postgres;
-- ddl-end --

-- object: public.gateway | type: TABLE --
-- DROP TABLE IF EXISTS public.gateway CASCADE;
CREATE TABLE public.gateway(
	gateway_id decimal(20,0) NOT NULL,
	home_name varchar(50) NOT NULL DEFAULT '',
	version integer DEFAULT -1,
	socket integer DEFAULT -1,
	home_timezone smallint NOT NULL DEFAULT 0,
	logging_days integer NOT NULL DEFAULT 7,
	CONSTRAINT adapters_pk PRIMARY KEY (gateway_id),
	CONSTRAINT check_positive_gw_id CHECK (gateway_id >= 0)

);
-- ddl-end --
ALTER TABLE public.gateway OWNER TO postgres;
-- ddl-end --

-- object: public.user_gateway | type: TABLE --
-- DROP TABLE IF EXISTS public.user_gateway CASCADE;
CREATE TABLE public.user_gateway(
	user_id integer NOT NULL,
	gateway_id decimal(20,0) NOT NULL,
	permission varchar(15) NOT NULL,
	accepted bool NOT NULL DEFAULT true,
	CONSTRAINT users_adapters_pk PRIMARY KEY (user_id,gateway_id)

);
-- ddl-end --
ALTER TABLE public.user_gateway OWNER TO postgres;
-- ddl-end --

-- object: public.users_user_id_seq | type: SEQUENCE --
-- DROP SEQUENCE IF EXISTS public.users_user_id_seq CASCADE;
CREATE SEQUENCE public.users_user_id_seq
	INCREMENT BY 1
	MINVALUE -2147483648
	MAXVALUE 2147483647
	START WITH 1
	CACHE 1
	NO CYCLE
	OWNED BY NONE;
-- ddl-end --

-- object: public.device | type: TABLE --
-- DROP TABLE IF EXISTS public.device CASCADE;
CREATE TABLE public.device(
	device_euid decimal(10) NOT NULL,
	device_type smallint NOT NULL,
	device_name varchar(50) NOT NULL DEFAULT '',
	refresh integer NOT NULL DEFAULT 10,
	battery smallint NOT NULL DEFAULT -1,
	quality integer NOT NULL DEFAULT -1,
	init char NOT NULL DEFAULT '0',
	involved bigint NOT NULL DEFAULT 0,
	measured_at bigint NOT NULL DEFAULT 0,
	location_id bigint,
	gateway_id decimal(20,0) NOT NULL,
	CONSTRAINT check_positive_mac CHECK (device_euid>= 0),
	CONSTRAINT facilities_pk PRIMARY KEY (device_euid)

);
-- ddl-end --
ALTER TABLE public.device OWNER TO postgres;
-- ddl-end --

-- object: public.module | type: TABLE --
-- DROP TABLE IF EXISTS public.module CASCADE;
CREATE TABLE public.module(
	device_euid decimal(10) NOT NULL,
	module_id smallint NOT NULL,
	measured_value real NOT NULL DEFAULT 0,
	CONSTRAINT devices_pk PRIMARY KEY (device_euid,module_id)

);
-- ddl-end --
ALTER TABLE public.module OWNER TO postgres;
-- ddl-end --

-- object: public.location | type: TABLE --
-- DROP TABLE IF EXISTS public.location CASCADE;
CREATE TABLE public.location(
	location_id bigserial NOT NULL,
	gateway_id decimal(20,0) NOT NULL,
	kind smallint NOT NULL,
	location_name varchar(50) NOT NULL DEFAULT '',
	CONSTRAINT rooms_pk PRIMARY KEY (location_id,gateway_id)

);
-- ddl-end --
ALTER TABLE public.location OWNER TO postgres;
-- ddl-end --

-- object: public.log | type: TABLE --
-- DROP TABLE IF EXISTS public.log CASCADE;
CREATE TABLE public.log(
	device_euid decimal(10) NOT NULL,
	module_id smallint NOT NULL,
	measured_at bigint NOT NULL,
	measured_value real NOT NULL,
	CONSTRAINT logs_pk PRIMARY KEY (measured_at,device_euid,module_id)

);
-- ddl-end --
ALTER TABLE public.log OWNER TO postgres;
-- ddl-end --

-- object: public.achievements | type: TABLE --
-- DROP TABLE IF EXISTS public.achievements CASCADE;
CREATE TABLE public.achievements(
	achievement_id integer NOT NULL,
	parent_id integer,
	category integer NOT NULL,
	points integer NOT NULL,
	progress_total integer NOT NULL,
	visibility boolean NOT NULL,
	range boolean NOT NULL,
	notification boolean NOT NULL,
	CONSTRAINT achievements_pk PRIMARY KEY (achievement_id)

);
-- ddl-end --
ALTER TABLE public.achievements OWNER TO postgres;
-- ddl-end --

-- object: public.notifications | type: TABLE --
-- DROP TABLE IF EXISTS public.notifications CASCADE;
CREATE TABLE public.notifications(
	message_id bigint NOT NULL,
	text xml NOT NULL,
	level smallint NOT NULL,
	"timestamp" bigint NOT NULL,
	fk_user_id integer NOT NULL,
	read boolean NOT NULL,
	name text NOT NULL,
	CONSTRAINT notifications_pk PRIMARY KEY (message_id)

);
-- ddl-end --
ALTER TABLE public.notifications OWNER TO postgres;
-- ddl-end --

-- object: public.mobile_devices | type: TABLE --
-- DROP TABLE IF EXISTS public.mobile_devices CASCADE;
CREATE TABLE public.mobile_devices(
	mobile_id text NOT NULL,
	fk_user_id integer NOT NULL,
	token text,
	type varchar(255),
	locale varchar(10),
	push_notification text,
	CONSTRAINT mobile_devices_pk PRIMARY KEY (mobile_id,fk_user_id)

);
-- ddl-end --
ALTER TABLE public.mobile_devices OWNER TO postgres;
-- ddl-end --

-- object: public.user_algorithm | type: TABLE --
-- DROP TABLE IF EXISTS public.user_algorithm CASCADE;
CREATE TABLE public.user_algorithm(
	user_id integer NOT NULL,
	algorithm_id integer NOT NULL,
	user_algorithm_id serial NOT NULL,
	parameters text NOT NULL,
	data text NOT NULL,
	name text NOT NULL,
	gateway_id decimal(20,0) NOT NULL,
	state text NOT NULL,
	CONSTRAINT users_algorithms_pk PRIMARY KEY (user_id,algorithm_id,user_algorithm_id)

);
-- ddl-end --
ALTER TABLE public.user_algorithm OWNER TO postgres;
-- ddl-end --

-- object: public.algorithm | type: TABLE --
-- DROP TABLE IF EXISTS public.algorithm CASCADE;
CREATE TABLE public.algorithm(
	algorithm_id integer NOT NULL,
	name varchar(50) NOT NULL,
	type integer NOT NULL,
	CONSTRAINT u_algorithms_pk PRIMARY KEY (algorithm_id)

);
-- ddl-end --
ALTER TABLE public.algorithm OWNER TO postgres;
-- ddl-end --

-- object: public.algorithm_gateway | type: TABLE --
-- DROP TABLE IF EXISTS public.algorithm_gateway CASCADE;
CREATE TABLE public.algorithm_gateway(
	gateway_id decimal(20,0) NOT NULL,
	email_list text NOT NULL,
	algorithm_id integer NOT NULL,
	CONSTRAINT algorithms_adapters_pk PRIMARY KEY (gateway_id,email_list)

);
-- ddl-end --
ALTER TABLE public.algorithm_gateway OWNER TO postgres;
-- ddl-end --

-- object: public.algorithm_device | type: TABLE --
-- DROP TABLE IF EXISTS public.algorithm_device CASCADE;
CREATE TABLE public.algorithm_device(
	user_algorithm_id integer NOT NULL,
	user_id integer NOT NULL,
	algorithm_id integer NOT NULL,
	device_euid decimal(10) NOT NULL,
	module_type smallint NOT NULL,
	algo_devices_id serial NOT NULL,
	CONSTRAINT algo_devices_pk PRIMARY KEY (user_algorithm_id,user_id,algorithm_id,device_euid,module_type,algo_devices_id)

);
-- ddl-end --
ALTER TABLE public.algorithm_device OWNER TO postgres;
-- ddl-end --

-- object: public.users_achievements | type: TABLE --
-- DROP TABLE IF EXISTS public.users_achievements CASCADE;
CREATE TABLE public.users_achievements(
	fk_achievement_id integer NOT NULL,
	fk_user_id integer NOT NULL,
	progress_current integer NOT NULL,
	date bigint,
	CONSTRAINT users_achievements_pk PRIMARY KEY (fk_achievement_id,fk_user_id)

);
-- ddl-end --
ALTER TABLE public.users_achievements OWNER TO postgres;
-- ddl-end --

-- object: public.adapters_achievements | type: TABLE --
-- DROP TABLE IF EXISTS public.adapters_achievements CASCADE;
CREATE TABLE public.adapters_achievements(
	fk_achievement_id integer NOT NULL,
	fk_adapter_id decimal(20,0) NOT NULL,
	progress_current integer NOT NULL,
	date bigint,
	CONSTRAINT adapters_achievements_pk PRIMARY KEY (fk_achievement_id,fk_adapter_id)

);
-- ddl-end --
ALTER TABLE public.adapters_achievements OWNER TO postgres;
-- ddl-end --

-- object: index_fb_id | type: INDEX --
-- DROP INDEX IF EXISTS public.index_fb_id CASCADE;
CREATE UNIQUE INDEX index_fb_id ON public.users
	USING btree
	(
	  facebook_id ASC NULLS FIRST
	);
-- ddl-end --

-- object: index_mail | type: INDEX --
-- DROP INDEX IF EXISTS public.index_mail CASCADE;
CREATE UNIQUE INDEX index_mail ON public.users
	USING btree
	(
	  google_id ASC NULLS FIRST
	);
-- ddl-end --

-- object: index_google_id | type: INDEX --
-- DROP INDEX IF EXISTS public.index_google_id CASCADE;
CREATE UNIQUE INDEX index_google_id ON public.users
	USING btree
	(
	  google_id ASC NULLS FIRST
	);
-- ddl-end --

-- object: index_logs_mttv | type: INDEX --
-- DROP INDEX IF EXISTS public.index_logs_mttv CASCADE;
CREATE INDEX index_logs_mttv ON public.log
	USING btree
	(
	  device_euid ASC NULLS LAST,
	  module_id ASC NULLS LAST,
	  measured_at ASC NULLS LAST,
	  measured_value ASC NULLS LAST
	);
-- ddl-end --
COMMENT ON INDEX public.index_logs_mttv IS 'mttv = mac, type, timestamp, value';
-- ddl-end --

-- object: public.push_notification_service | type: TABLE --
-- DROP TABLE IF EXISTS public.push_notification_service CASCADE;
CREATE TABLE public.push_notification_service(
	push_notification_service_id serial NOT NULL,
	user_id integer,
	service_reference_id text,
	CONSTRAINT pk_push_notification PRIMARY KEY (push_notification_service_id)

);
-- ddl-end --
ALTER TABLE public.push_notification_service OWNER TO postgres;
-- ddl-end --

-- object: adapters_users_adapters | type: CONSTRAINT --
-- ALTER TABLE public.user_gateway DROP CONSTRAINT IF EXISTS adapters_users_adapters CASCADE;
ALTER TABLE public.user_gateway ADD CONSTRAINT adapters_users_adapters FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: users_adapters_users | type: CONSTRAINT --
-- ALTER TABLE public.user_gateway DROP CONSTRAINT IF EXISTS users_adapters_users CASCADE;
ALTER TABLE public.user_gateway ADD CONSTRAINT users_adapters_users FOREIGN KEY (user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: facilities_adapters | type: CONSTRAINT --
-- ALTER TABLE public.device DROP CONSTRAINT IF EXISTS facilities_adapters CASCADE;
ALTER TABLE public.device ADD CONSTRAINT facilities_adapters FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: facilities_rooms | type: CONSTRAINT --
-- ALTER TABLE public.device DROP CONSTRAINT IF EXISTS facilities_rooms CASCADE;
ALTER TABLE public.device ADD CONSTRAINT facilities_rooms FOREIGN KEY (location_id,gateway_id)
REFERENCES public.location (location_id,gateway_id) MATCH SIMPLE
ON DELETE SET NULL ON UPDATE NO ACTION;
-- ddl-end --

-- object: devices_facilities | type: CONSTRAINT --
-- ALTER TABLE public.module DROP CONSTRAINT IF EXISTS devices_facilities CASCADE;
ALTER TABLE public.module ADD CONSTRAINT devices_facilities FOREIGN KEY (device_euid)
REFERENCES public.device (device_euid) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: rooms_adapters | type: CONSTRAINT --
-- ALTER TABLE public.location DROP CONSTRAINT IF EXISTS rooms_adapters CASCADE;
ALTER TABLE public.location ADD CONSTRAINT rooms_adapters FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: logs_devices | type: CONSTRAINT --
-- ALTER TABLE public.log DROP CONSTRAINT IF EXISTS logs_devices CASCADE;
ALTER TABLE public.log ADD CONSTRAINT logs_devices FOREIGN KEY (device_euid,module_id)
REFERENCES public.module (device_euid,module_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: achievements_achievements | type: CONSTRAINT --
-- ALTER TABLE public.achievements DROP CONSTRAINT IF EXISTS achievements_achievements CASCADE;
ALTER TABLE public.achievements ADD CONSTRAINT achievements_achievements FOREIGN KEY (parent_id)
REFERENCES public.achievements (achievement_id) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;
-- ddl-end --

-- object: notifications_users | type: CONSTRAINT --
-- ALTER TABLE public.notifications DROP CONSTRAINT IF EXISTS notifications_users CASCADE;
ALTER TABLE public.notifications ADD CONSTRAINT notifications_users FOREIGN KEY (fk_user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: mobile_devices_users | type: CONSTRAINT --
-- ALTER TABLE public.mobile_devices DROP CONSTRAINT IF EXISTS mobile_devices_users CASCADE;
ALTER TABLE public.mobile_devices ADD CONSTRAINT mobile_devices_users FOREIGN KEY (fk_user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: users_algorithms_users | type: CONSTRAINT --
-- ALTER TABLE public.user_algorithm DROP CONSTRAINT IF EXISTS users_algorithms_users CASCADE;
ALTER TABLE public.user_algorithm ADD CONSTRAINT users_algorithms_users FOREIGN KEY (user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: users_algorithms_algorithms | type: CONSTRAINT --
-- ALTER TABLE public.user_algorithm DROP CONSTRAINT IF EXISTS users_algorithms_algorithms CASCADE;
ALTER TABLE public.user_algorithm ADD CONSTRAINT users_algorithms_algorithms FOREIGN KEY (algorithm_id)
REFERENCES public.algorithm (algorithm_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: users_algorithms_adapters | type: CONSTRAINT --
-- ALTER TABLE public.user_algorithm DROP CONSTRAINT IF EXISTS users_algorithms_adapters CASCADE;
ALTER TABLE public.user_algorithm ADD CONSTRAINT users_algorithms_adapters FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: algorithms_adapters_adapters | type: CONSTRAINT --
-- ALTER TABLE public.algorithm_gateway DROP CONSTRAINT IF EXISTS algorithms_adapters_adapters CASCADE;
ALTER TABLE public.algorithm_gateway ADD CONSTRAINT algorithms_adapters_adapters FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: algorithms_adapters_u_algorithms | type: CONSTRAINT --
-- ALTER TABLE public.algorithm_gateway DROP CONSTRAINT IF EXISTS algorithms_adapters_u_algorithms CASCADE;
ALTER TABLE public.algorithm_gateway ADD CONSTRAINT algorithms_adapters_u_algorithms FOREIGN KEY (algorithm_id)
REFERENCES public.algorithm (algorithm_id) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --

-- object: algo_devices_devices | type: CONSTRAINT --
-- ALTER TABLE public.algorithm_device DROP CONSTRAINT IF EXISTS algo_devices_devices CASCADE;
ALTER TABLE public.algorithm_device ADD CONSTRAINT algo_devices_devices FOREIGN KEY (device_euid,module_type)
REFERENCES public.module (device_euid,module_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_algo_devices_users_algorithms | type: CONSTRAINT --
-- ALTER TABLE public.algorithm_device DROP CONSTRAINT IF EXISTS fk_algo_devices_users_algorithms CASCADE;
ALTER TABLE public.algorithm_device ADD CONSTRAINT fk_algo_devices_users_algorithms FOREIGN KEY (user_id,algorithm_id,user_algorithm_id)
REFERENCES public.user_algorithm (user_id,algorithm_id,user_algorithm_id) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --

-- object: achievement_detail_achievements | type: CONSTRAINT --
-- ALTER TABLE public.users_achievements DROP CONSTRAINT IF EXISTS achievement_detail_achievements CASCADE;
ALTER TABLE public.users_achievements ADD CONSTRAINT achievement_detail_achievements FOREIGN KEY (fk_achievement_id)
REFERENCES public.achievements (achievement_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: achievement_detail_users | type: CONSTRAINT --
-- ALTER TABLE public.users_achievements DROP CONSTRAINT IF EXISTS achievement_detail_users CASCADE;
ALTER TABLE public.users_achievements ADD CONSTRAINT achievement_detail_users FOREIGN KEY (fk_user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: adapters_achievements_achievements | type: CONSTRAINT --
-- ALTER TABLE public.adapters_achievements DROP CONSTRAINT IF EXISTS adapters_achievements_achievements CASCADE;
ALTER TABLE public.adapters_achievements ADD CONSTRAINT adapters_achievements_achievements FOREIGN KEY (fk_achievement_id)
REFERENCES public.achievements (achievement_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: adapters_achievements_adapters | type: CONSTRAINT --
-- ALTER TABLE public.adapters_achievements DROP CONSTRAINT IF EXISTS adapters_achievements_adapters CASCADE;
ALTER TABLE public.adapters_achievements ADD CONSTRAINT adapters_achievements_adapters FOREIGN KEY (fk_adapter_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_user_push_notification | type: CONSTRAINT --
-- ALTER TABLE public.push_notification_service DROP CONSTRAINT IF EXISTS fk_user_push_notification CASCADE;
ALTER TABLE public.push_notification_service ADD CONSTRAINT fk_user_push_notification FOREIGN KEY (user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: grant_b8cdb58d0f | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.gateway
   TO hardware;
-- ddl-end --

-- object: grant_d5aa0714b7 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.device
   TO hardware;
-- ddl-end --

-- object: grant_e9d429f0e0 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.module
   TO hardware;
-- ddl-end --

-- object: grant_6cad8f1723 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.log
   TO hardware;
-- ddl-end --

-- object: grant_5f42572af9 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.users
   TO user_data;
-- ddl-end --

-- object: grant_3be3efbbe8 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.user_gateway
   TO user_data;
-- ddl-end --

-- object: grant_6bf4173d08 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.location
   TO user_data;
-- ddl-end --

-- object: grant_72ab7fa449 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.algorithm
   TO algorithms;
-- ddl-end --

-- object: grant_3b45d69d6e | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.algorithm_gateway
   TO algorithms;
-- ddl-end --

-- object: grant_8315ee0a96 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.algorithm_device
   TO algorithms;
-- ddl-end --

-- object: grant_379538b7d4 | type: PERMISSION --
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.push_notification_service
   TO user_data;
-- ddl-end --


