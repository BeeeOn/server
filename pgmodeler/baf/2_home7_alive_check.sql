-- File:   2_home7_alive_check.sql
-- Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
-- Created on 23. March 2016

-- SQL to create tables of task AliveCheck.
-- A prerequisite is existence of BAF tables and public.gateway table (column gateway_id).

-- object: public.task_alive_check | type: TABLE --
-- DROP TABLE IF EXISTS public.task_alive_check CASCADE;
CREATE TABLE public.task_alive_check(
	instance_id integer NOT NULL,
	notifications bool NOT NULL,
	gateway_id decimal(20,0) NOT NULL,
	CONSTRAINT task_alive_check_pk PRIMARY KEY (instance_id)

);

-- Set owner of table.
ALTER TABLE public.task_alive_check OWNER TO postgres;

-- object: instance_id_fk | type: CONSTRAINT --
-- ALTER TABLE public.task_alive_check DROP CONSTRAINT IF EXISTS instance_id_fk CASCADE;
ALTER TABLE public.task_alive_check ADD CONSTRAINT instance_id_fk FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;

-- object: rel_task_alive_check_gateway | type: CONSTRAINT --
-- ALTER TABLE public.task_alive_check DROP CONSTRAINT IF EXISTS rel_task_alive_check_gateway CASCADE;
ALTER TABLE public.task_alive_check ADD CONSTRAINT rel_task_alive_check_gateway FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;
