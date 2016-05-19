-- File:   task_fire_hazard.sql
-- Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
-- Created on 4. May 2016

-- SQL to create table task_fire_hazard for BAF automation task FireHazard.

-- object: public.task_fire_hazard | type: TABLE --
-- DROP TABLE IF EXISTS public.task_fire_hazard CASCADE;
CREATE TABLE public.task_fire_hazard(
	instance_id integer NOT NULL,
	value integer,
	device_euid decimal(10),
	module_id smallint,
	a_device_euid decimal(10),
	a_module_id smallint,
	CONSTRAINT task_fire_hazard_pk PRIMARY KEY (instance_id)

);
-- Set owner of table
ALTER TABLE public.task_fire_hazard OWNER TO postgres;

-- object: rel_task_fire_hazard_instance | type: CONSTRAINT --
-- ALTER TABLE public.task_fire_hazard DROP CONSTRAINT IF EXISTS rel_task_fire_hazard_instance CASCADE;
ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_instance FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: rel_task_fire_hazard_device | type: CONSTRAINT --
-- ALTER TABLE public.task_fire_hazard DROP CONSTRAINT IF EXISTS rel_task_fire_hazard_device CASCADE;
ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_device FOREIGN KEY (device_euid)
REFERENCES public.device (device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;
-- ddl-end --

-- object: rel_task_fire_hazard_device2 | type: CONSTRAINT --
-- ALTER TABLE public.task_fire_hazard DROP CONSTRAINT IF EXISTS rel_task_fire_hazard_device2 CASCADE;
ALTER TABLE public.task_fire_hazard ADD CONSTRAINT rel_task_fire_hazard_device2 FOREIGN KEY (a_device_euid)
REFERENCES public.device (device_euid) MATCH FULL
ON DELETE SET NULL ON UPDATE NO ACTION;