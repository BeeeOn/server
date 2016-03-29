-- File:   1_home7_baf_tables.sql
-- Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
-- Created on 23. March 2016

-- SQL to create tables for BAF - BeeeOn Automation Framework.
-- A prerequisite is existence of public.user table (column user_id).

-- object: public.task | type: TABLE --
-- DROP TABLE IF EXISTS public.task CASCADE;
CREATE TABLE public.task(
	task_id smallint NOT NULL,
	name varchar(20),
	version smallint,
	type varchar(8),
	library_path text,
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
	instance_id integer,
	permission varchar(15),
	relative_id smallint NOT NULL,
	CONSTRAINT user_instance_pk PRIMARY KEY (user_id,relative_id)
);

-- Set owner of tables.
ALTER TABLE public.task OWNER TO postgres;
ALTER TABLE public.instance OWNER TO postgres;
ALTER TABLE public.user_instance OWNER TO postgres;

-- object: task_id_fk | type: CONSTRAINT --
-- ALTER TABLE public.instance DROP CONSTRAINT IF EXISTS task_id_fk CASCADE;
ALTER TABLE public.instance ADD CONSTRAINT task_id_fk FOREIGN KEY (task_id)
REFERENCES public.task (task_id) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;

-- object: instance_id_fk | type: CONSTRAINT --
-- ALTER TABLE public.user_instance DROP CONSTRAINT IF EXISTS instance_id_fk CASCADE;
ALTER TABLE public.user_instance ADD CONSTRAINT instance_id_fk FOREIGN KEY (instance_id)
REFERENCES public.instance (instance_id) MATCH FULL
ON DELETE NO ACTION ON UPDATE NO ACTION;
