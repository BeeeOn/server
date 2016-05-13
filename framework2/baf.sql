-- File:   baf.sql
-- Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
-- Created on 20. April 2016

-- SQL to create tables for BAF - BeeeOn Automation Framework.

-- Table to store task information.
CREATE TABLE public.task(
	task_id smallint NOT NULL,
	name varchar(20) NOT NULL,
	version smallint NOT NULL,
	type varchar(10) NOT NULL,
	CONSTRAINT task_pk PRIMARY KEY (task_id)
);

-- Table to store instances.
CREATE TABLE public.instance(
	instance_id serial NOT NULL,
	user_id integer,
	task_id smallint,
	CONSTRAINT instance_pk PRIMARY KEY (instance_id)
);

 -- Table to store configuration of task AliveCheck.
CREATE TABLE public.task_alive_check(
	instance_id integer NOT NULL,
	gateway_id decimal(20,0),
	send_notif bool NOT NULL,
	CONSTRAINT task_alive_check_pk PRIMARY KEY (instance_id)
);

 -- Table to store configuration of task Watchdog.
CREATE TABLE public.task_watchdog(
	instance_id integer NOT NULL,
	type varchar(6) NOT NULL,
	gateway_id decimal(20,0),
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

 -- Table to store configuration of task FireHazard.
CREATE TABLE public.task_fire_hazard(
	instance_id integer NOT NULL,
	value integer,
	gateway_id decimal(20,0),
	device_euid decimal(10),
	module_id smallint,
	a_gateway_id decimal(20,0),
	a_device_euid decimal(10),
	a_module_id smallint,
	CONSTRAINT task_fire_hazard_pk PRIMARY KEY (instance_id)
);

-- Set owner of tables to postgres user.
ALTER TABLE public.task OWNER TO postgres;
ALTER TABLE public.instance OWNER TO postgres;
ALTER TABLE public.task_alive_check OWNER TO postgres;
ALTER TABLE public.task_watchdog OWNER TO postgres;
ALTER TABLE public.task_fire_hazard OWNER TO postgres;

-- Set foreign keys.
ALTER TABLE public.instance ADD CONSTRAINT task_id_fk FOREIGN KEY (task_id)
REFERENCES public.task (task_id) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.instance ADD CONSTRAINT rel_user_instance FOREIGN KEY (user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

ALTER TABLE public.task_alive_check ADD CONSTRAINT instance_id_fk FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

ALTER TABLE public.task_alive_check ADD CONSTRAINT rel_task_alive_check_gateway FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_instance FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_device FOREIGN KEY (gateway_id,device_euid)
REFERENCES public.device (gateway_id,device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_device_act FOREIGN KEY (a_gateway_id,a_device_euid)
REFERENCES public.device (gateway_id,device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_module FOREIGN KEY (module_id, device_euid)
REFERENCES public.module (module_id, device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_module_act FOREIGN KEY (a_module_id,a_device_euid)
REFERENCES public.module (module_id,device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_instance FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_device FOREIGN KEY (gateway_id,device_euid)
REFERENCES public.device (gateway_id,device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_device_act FOREIGN KEY (a_gateway_id,a_device_euid)
REFERENCES public.device (gateway_id,device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_module FOREIGN KEY (module_id, device_euid)
REFERENCES public.module (module_id, device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;

ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_module_act FOREIGN KEY (a_module_id,a_device_euid)
REFERENCES public.module (module_id,device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;