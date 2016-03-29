-- File:   2_home7_watchdog.sql
-- Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
-- Created on 29. March 2016

-- SQL to create tables of task Watchdog.
-- A prerequisite is existence of BAF tables and public.gateway table (column gateway_id).

-- object: public.task_watchdog | type: TABLE --
-- DROP TABLE IF EXISTS public.task_watchdog CASCADE;
CREATE TABLE public.task_watchdog(
	instance_id integer NOT NULL,
	device_euid decimal(10) NOT NULL,
	module_id smallint NOT NULL,
	operator varchar(7) NOT NULL,
	value real NOT NULL,
	notification_text text,
	CONSTRAINT task_watchdog_pk PRIMARY KEY (instance_id)
);

-- Set owner of table.
ALTER TABLE public.task_watchdog OWNER TO postgres;

-- object: rel_task_watchdog_instance | type: CONSTRAINT --
-- ALTER TABLE public.task_watchdog DROP CONSTRAINT IF EXISTS rel_task_watchdog_instance CASCADE;
ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_instance FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;

-- object: rel_task_watchdog_device | type: CONSTRAINT --
-- ALTER TABLE public.task_watchdog DROP CONSTRAINT IF EXISTS rel_task_watchdog_device CASCADE;
ALTER TABLE public.task_watchdog ADD CONSTRAINT rel_task_watchdog_device FOREIGN KEY (device_euid)
REFERENCES public.device (device_euid) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;
