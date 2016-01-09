DROP INDEX index_mail;

CREATE UNIQUE INDEX index_mail ON public.users
	USING btree
	(
	  mail ASC NULLS FIRST
	);