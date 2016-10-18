DROP TABLE IF EXISTS places;
CREATE TABLE places (
	id      ${type_uuid} NOT NULL primary key,
	name    varchar(250)
);

DROP TABLE IF EXISTS gateways;
CREATE TABLE gateways (
	id          decimal(16, 0) NOT NULL primary key,
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
	accessToken varchar(250),
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
