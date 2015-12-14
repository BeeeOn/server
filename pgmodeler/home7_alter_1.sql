ALTER TABLE gateway ADD COLUMN altitude REAL DEFAULT 0;
ALTER TABLE gateway ADD COLUMN longitude double precision DEFAULT 0;
ALTER TABLE gateway ADD COLUMN latitude double precision DEFAULT 0;


CREATE TABLE public.notification(
	notification_id bigserial NOT NULL,
	message xml NOT NULL,
	category smallint NOT NULL,
	created_at bigint NOT NULL,
	user_id integer NOT NULL,
	was_read boolean NOT NULL,
	name text NOT NULL,
	CONSTRAINT notification_pk PRIMARY KEY (notification_id)
);

ALTER TABLE public.notification ADD CONSTRAINT notification_user FOREIGN KEY (user_id)
REFERENCES public.users (user_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;



DROP TABLE IF EXISTS notifications;