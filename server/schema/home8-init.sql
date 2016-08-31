DROP TABLE IF EXISTS places;
CREATE TABLE places (
	id      ${type_uuid} NOT NULL primary key,
	name    varchar(250)
);

DROP TABLE IF EXISTS gateways;
CREATE TABLE gateways (
	id          ${type_int64} NOT NULL primary key,
	name        varchar(250),
	place_id    ${type_uuid} REFERENCES places(id),
	altitude    ${type_double},
	latitude    ${type_double},
	longitude   ${type_double}
);

DROP TABLE IF EXISTS locations;
CREATE TABLE locations (
	id          ${type_uuid} NOT NULL primary key,
	name        varchar(250),
	place_id    ${type_uuid} REFERENCES places(id)
);

DROP TABLE IF EXISTS users;
CREATE TABLE users (
	id           ${type_uuid} NOT NULL primary key,
	first_name   varchar(250) NOT NULL,
	last_name    varchar(250) NOT NULL
);

DROP TABLE IF EXISTS identities;
CREATE TABLE identities (
	id          ${type_uuid} NOT NULL primary key,
	email       varchar(250) NOT NULL,
	CONSTRAINT unique_identity UNIQUE (email)
);

DROP TABLE IF EXISTS verified_identities;
CREATE TABLE verified_identities (
	id          ${type_uuid} NOT NULL primary key,
	identity_id ${type_uuid} NOT NULL REFERENCES identities(id),
	user_id     ${type_uuid} NOT NULL REFERENCES users(id),
	provider    varchar(250) NOT NULL,
	picture     varchar(250),
	access_token varchar(250),
	CONSTRAINT unique_verified_identity UNIQUE (identity_id, provider)
);

DROP TABLE IF EXISTS roles_in_place;
CREATE TABLE roles_in_place (
	id           ${type_uuid} NOT NULL primary key,
	place_id     ${type_uuid} NOT NULL REFERENCES places(id),
	identity_id  ${type_uuid} NOT NULL REFERENCES identities(id),
	level        ${type_smallint} NOT NULL,
	CONSTRAINT check_valid_level CHECK (level >= 0 AND level <= 100)
);

DROP TABLE IF EXISTS devices;
CREATE TABLE devices (
	id           ${type_int64} NOT NULL,
	gateway_id   ${type_int64} NOT NULL REFERENCES gateways(id),
	location_id  uuid REFERENCES locations(id),
	name         varchar(250) NOT NULL,
	type         smallint NOT NULL,
	refresh      integer NOT NULL DEFAULT 10,
	battery      smallint,
	signal       smallint,
	first_seen   bigint NOT NULL,
	last_seen    bigint NOT NULL,
	active_since bigint,
	CONSTRAINT devices_pk PRIMARY KEY (id, gateway_id),
	CONSTRAINT check_id_positive CHECK (id >= 0),
	CONSTRAINT check_seen_valid CHECK (first_seen <= last_seen),
	CONSTRAINT check_active_valid CHECK (
		active_since IS NULL OR first_seen <= active_since),
	CONSTRAINT check_battery_valid CHECK (
		battery IS NULL OR (battery >= 0 AND battery <= 100)),
	CONSTRAINT check_signal_valid CHECK (
		signal IS NULL OR (signal >= 0 AND signal <= 100))
);
