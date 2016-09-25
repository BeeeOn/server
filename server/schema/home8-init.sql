DROP TABLE IF EXISTS places;
CREATE TABLE places (
	id      uuid NOT NULL primary key,
	name    varchar(250)
);

DROP TABLE IF EXISTS gateways;
CREATE TABLE gateways (
	id          uuid NOT NULL primary key,
	name        varchar(250),
	place_id    uuid REFERENCES places(id),
	altitude    double precision,
	latitude    double precision,
	longitude   double precision
);
