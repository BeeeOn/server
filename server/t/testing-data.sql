-- Initialization script for PostgreSQL engine.
-- It is necessary to apply this script for testing.

BEGIN;

INSERT INTO beeeon.users (id, first_name, last_name, locale)
VALUES
(
	'99e54009-1b7d-4408-9bb9-d9fb0c69d5b4',
	'Joe',
	'Doe',
	'en_US'
),
(
	'ade9b91f-d85b-48d8-8dc8-caa210e78058',
	'John',
	'Smith',
	'en_US'
);

INSERT INTO beeeon.identities (id, email)
VALUES
('be572079-0971-47f0-95df-68476a642911', 'joe.doe@example.org'),
('c152dfcb-59f4-4e50-aeed-b78afcd9d2a1', 'john.smith@example.org');

INSERT INTO beeeon.verified_identities (id, identity_id, user_id, provider, picture, access_token)
VALUES
(
	'2e0a2555-b5ae-42aa-9fd1-1923c5863a6a',
	'be572079-0971-47f0-95df-68476a642911',
	'99e54009-1b7d-4408-9bb9-d9fb0c69d5b4',
	'prepared',
	NULL,
	NULL
),
(
	'ab9996fa-5252-4df5-a3f6-1d7efde7a021',
	'c152dfcb-59f4-4e50-aeed-b78afcd9d2a1',
	'ade9b91f-d85b-48d8-8dc8-caa210e78058',
	'prepared',
	NULL,
	NULL
);

INSERT INTO beeeon.gateways (id, name, altitude, latitude, longitude)
VALUES
(
	1284174504043136,
	$$ Joe Doe's Gateway $$, --' highlighting hack
	NULL,
	NULL,
	NULL
),
(
	1780053541714013,
	'Gateway To Un/assign',
	NULL,
	NULL,
	NULL
);

INSERT INTO beeeon.locations (id, name, gateway_id)
VALUES
(
	'ad01fa7b-0295-4c56-b84c-62372112762f',
	'Kitchen #1',
	1284174504043136
),
(
	'93e56107-686e-4e87-ab61-9c83283a2057',
	'Livingroom #1',
	1284174504043136
),
(
	'dd74afbf-03ee-40bd-8289-08df10196ecd',
	'Bathroom #1',
	1284174504043136
),
(
	'fe6a144a-33f4-4405-8ab4-5c7af8eec540',
	'Hall #1',
	1284174504043136
);

INSERT INTO beeeon.devices (
	id,
	gateway_id,
	location_id,
	name,
	type,
	refresh,
	battery,
	signal,
	first_seen,
	last_seen,
	active_since
)
VALUES
(
	beeeon.to_device_id(11760534700801991502),
	1284174504043136,
	'ad01fa7b-0295-4c56-b84c-62372112762f',
	'Temperature',
	0,
	5,
	50,
	90,
	timestamp '2015-4-9 15:43:01',
	timestamp '2016-9-1 13:27:18',
	timestamp '2015-5-2 17:59:59'
),
(
	beeeon.to_device_id(11780870713433535053),
	1284174504043136,
	'93e56107-686e-4e87-ab61-9c83283a2057',
	'Humidity',
	0,
	1000,
	99,
	45,
	timestamp '2016-8-8 08:08:08',
	NOW() AT TIME ZONE 'UTC',
	timestamp '2016-8-9 08:09:08'
),
(
	beeeon.to_device_id(11758097814818974973),
	1284174504043136,
	'dd74afbf-03ee-40bd-8289-08df10196ecd',
	'Multi-sensor',
	3,
	5,
	50,
	90,
	timestamp '2016-9-10 11:12:13',
	timestamp '2016-10-10 11:11:22',
	timestamp '2016-9-10 11:30:01'
),
(
	beeeon.to_device_id(11777358992112902542),
	1284174504043136,
	NULL,
	'Unknown',
	4,
	2,
	NULL,
	NULL,
	NOW() AT TIME ZONE 'UTC' - interval '100 seconds',
	NOW() AT TIME ZONE 'UTC' - interval '7 seconds',
	NULL
);

INSERT INTO beeeon.device_properties (
	device_id,
	gateway_id,
	key,
	value,
	params
)
VALUES
(
	beeeon.to_device_id(11760534700801991502),
	1284174504043136,
	3, -- firmware
	'v1.0-6453',
	NULL
),
(
	beeeon.to_device_id(11780870713433535053),
	1284174504043136,
	3, -- firmware
	'000-111',
	NULL
),
(
	beeeon.to_device_id(11758097814818974973),
	1284174504043136,
	1, -- IP address
	'NPG6A1VaVcA4A7heqERjEg==',
	'{"it": 2000, "name": "aes256", "salt": "hwMCpcIm3lg4YDsrIRdvmAv-1z]8Qzfb"}'
),
(
	beeeon.to_device_id(11758097814818974973),
	1284174504043136,
	2, -- password
	'qmdPau6zKHxkTqnU3f7po/RiLNFTECFiA5BE7ICQZ8U=',
	'{"it": 2000, "name": "aes256", "salt": ",wXuaWo813cOsPHLV9likt3gh6V8HDG^"}'
);

---
-- Insert a big set of constant testing data.
-- The data set starts 8 days ago and contains data every 260 seconds.
---
INSERT INTO beeeon.sensor_history_recent (
	gateway_id,
	device_id,
	module_id,
	at,
	value
)
SELECT
	1284174504043136,
	beeeon.to_device_id(11760534700801991502),
	0,
	NOW() AT TIME ZONE 'UTC' - (i * interval '260 seconds'),
	19.5
FROM generate_series(0, (extract(epoch FROM interval '8 days') / 260)::integer - 1) AS i;

---
-- Insert a big set of sin(x) testing data.
-- The data set starts 2 hours ago and contains data every 110 seconds.
---
INSERT INTO beeeon.sensor_history_recent (
	gateway_id,
	device_id,
	module_id,
	at,
	value
)
SELECT
	1284174504043136,
	beeeon.to_device_id(11760534700801991502),
	1,
	NOW() AT TIME ZONE 'UTC' - (i * interval '110 seconds'),
	25 * SIN(i * 0.01)
FROM generate_series(0, (extract(epoch FROM interval '2 hours') / 110)::integer - 1) AS i;

---
-- Insert a big set of random testing data.
-- The data set starts 14 days ago and contains data every 720 seconds.
---
INSERT INTO beeeon.sensor_history_recent (
	gateway_id,
	device_id,
	module_id,
	at,
	value
)
SELECT
	1284174504043136,
	beeeon.to_device_id(11760534700801991502),
	2,
	NOW() AT TIME ZONE 'UTC' - (i * interval '720 seconds'),
	random() * 100
FROM generate_series(0, (extract(epoch FROM interval '14 days') / 720)::integer - 1) AS i;

COMMIT;
