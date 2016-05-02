-- File:   home7_baf.sql
-- Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
-- Created on 4. April 2016

-- SQL to create tables for BAF - BeeeOn Automation Framework.
/*
CREATE ROLE tasks;
-- Create baf role to login with.
CREATE ROLE baf WITH LOGIN PASSWORD '1234' IN ROLE tasks;
-- Grant select to all tables to baf role.
GRANT SELECT ON ALL TABLES IN SCHEMA public TO baf;
GRANT USAGE ON ALL SEQUENCES IN SCHEMA public TO baf;
*/
-- object: public.task | type: TABLE --
-- DROP TABLE IF EXISTS public.task CASCADE;
CREATE TABLE public.task(
	task_id smallint NOT NULL,
	name varchar(20) NOT NULL,
	version smallint NOT NULL,
	type varchar(10) NOT NULL,
	CONSTRAINT task_pk PRIMARY KEY (task_id)
);

-- object: public.instance | type: TABLE --
-- DROP TABLE IF EXISTS public.instance CASCADE;
CREATE TABLE public.instance(
instance_id serial NOT NULL,
task_id smallint,
CONSTRAINT instance_pk PRIMARY KEY (instance_id)
);

-- object: public.user_instance | type: TABLE --
-- DROP TABLE IF EXISTS public.user_instance CASCADE;
CREATE TABLE public.user_instance(
user_id integer NOT NULL,
instance_id integer NOT NULL,
permission varchar(10),
CONSTRAINT user_instance_pk PRIMARY KEY (user_id,instance_id)
);

-- object: public.task_alive_check | type: TABLE --
-- DROP TABLE IF EXISTS public.task_alive_check CASCADE;
CREATE TABLE public.task_alive_check(
instance_id integer NOT NULL,
gateway_id decimal(20,0),
send_notif bool NOT NULL,
CONSTRAINT task_alive_check_pk PRIMARY KEY (instance_id)
);

-- object: public.task_watchdog | type: TABLE --
-- DROP TABLE IF EXISTS public.task_watchdog CASCADE;
CREATE TABLE public.task_watchdog(
instance_id integer NOT NULL,
type varchar(6) NOT NULL,
device_euid decimal(10),
module_id smallint NOT NULL,
comp_operator varchar(2) NOT NULL,
value real NOT NULL,
notification text,
a_gateway_id decimal(20,0),
a_device_euid decimal(10),
a_module_id smallint,
a_value integer,
CONSTRAINT task_watchdog_pk PRIMARY KEY (instance_id)
);

-- Set owner of tables
ALTER TABLE public.task OWNER TO postgres;
ALTER TABLE public.instance OWNER TO postgres;
ALTER TABLE public.user_instance OWNER TO postgres;
ALTER TABLE public.task_alive_check OWNER TO postgres;
ALTER TABLE public.task_watchdog OWNER TO postgres;

-- object: task_id_fk | type: CONSTRAINT --
-- ALTER TABLE public.instance DROP CONSTRAINT IF EXISTS task_id_fk CASCADE;
ALTER TABLE public.instance ADD CONSTRAINT task_id_fk FOREIGN KEY (task_id)
REFERENCES public.task (task_id) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

-- object: user_id_fk | type: CONSTRAINT --
-- ALTER TABLE public.user_instance DROP CONSTRAINT IF EXISTS user_id_fk CASCADE;
ALTER TABLE public.user_instance ADD CONSTRAINT user_id_fk FOREIGN KEY (user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

-- object: instance_id_fk | type: CONSTRAINT --
-- ALTER TABLE public.user_instance DROP CONSTRAINT IF EXISTS instance_id_fk CASCADE;
ALTER TABLE public.user_instance ADD CONSTRAINT instance_id_fk FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

-- object: instance_id_fk | type: CONSTRAINT --
-- ALTER TABLE public.task_alive_check DROP CONSTRAINT IF EXISTS instance_id_fk CASCADE;
ALTER TABLE public.task_alive_check ADD CONSTRAINT instance_id_fk FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

-- object: rel_task_alive_check_gateway | type: CONSTRAINT --
-- ALTER TABLE public.task_alive_check DROP CONSTRAINT IF EXISTS rel_task_alive_check_gateway CASCADE;
ALTER TABLE public.task_alive_check ADD CONSTRAINT rel_task_alive_check_gateway FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

-- object: rel_task_watchdog_instance | type: CONSTRAINT --
-- ALTER TABLE public.task_watchdog DROP CONSTRAINT IF EXISTS rel_task_watchdog_instance CASCADE;
ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_instance FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

-- object: rel_task_watchdog_device | type: CONSTRAINT --
-- ALTER TABLE public.task_watchdog DROP CONSTRAINT IF EXISTS rel_task_watchdog_device CASCADE;
ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_device FOREIGN KEY (device_euid)
REFERENCES public.device (device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

-- object: rel_task_watchdog_device2 | type: CONSTRAINT --
-- ALTER TABLE public.task_watchdog DROP CONSTRAINT IF EXISTS rel_task_watchdog_device2 CASCADE;
ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_device2 FOREIGN KEY (a_device_euid)
REFERENCES public.device (device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

-- object: rel_task_watchdog_gateway | type: CONSTRAINT --
-- ALTER TABLE public.task_watchdog DROP CONSTRAINT IF EXISTS rel_task_watchdog_gateway CASCADE;
ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_gateway FOREIGN KEY (a_gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

/*
-- Grant priviledges to tasks role.
GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.task
   TO tasks;

GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.instance
   TO tasks;

GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.user_instance
   TO tasks;

GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.task_watchdog
   TO tasks;

GRANT SELECT,INSERT,UPDATE,DELETE
   ON TABLE public.task_alive_check
   TO tasks;
*/