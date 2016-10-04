DROP TABLE IF EXISTS places;
CREATE TABLE places (
	id      uuid NOT NULL primary key,
	name    varchar(250)
);

DROP TABLE IF EXISTS gateways;
CREATE TABLE gateways (
	id          decimal(16, 0) NOT NULL primary key,
	name        varchar(250),
	place_id    uuid REFERENCES places(id),
	altitude    double precision,
	latitude    double precision,
	longitude   double precision
);

DROP TABLE IF EXISTS locations;
CREATE TABLE locations (
	id          uuid NOT NULL primary key,
	name        varchar(250),
	place_id    uuid REFERENCES places(id)
);

DROP TABLE IF EXISTS users;
CREATE TABLE users (
	id           uuid NOT NULL primary key,
	first_name   varchar(250) NOT NULL,
	last_name    varchar(250) NOT NULL
);

DROP TABLE IF EXISTS identities;
CREATE TABLE identities (
	id          uuid NOT NULL primary key,
	email       varchar(250) NOT NULL,
	CONSTRAINT unique_identity UNIQUE (email)
);

DROP TABLE IF EXISTS verified_identities;
CREATE TABLE verified_identities (
	id          uuid NOT NOT primary key;
	identity_id uuid NOT NULL REFERENCES identities(id),
	user_id     uuid NOT NULL REFERENCES users(id),
	provider    varchar(250) NOT NULL,
	picture     varchar(250),
	accessToken varchar(250)
	CONSTRAINT unique_verified_identity UNIQUE (identity_id, provider)
);
