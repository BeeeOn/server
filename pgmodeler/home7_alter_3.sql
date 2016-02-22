ALTER TABLE module DROP COLUMN not_available;

CREATE TYPE gateway_status AS ENUM ('available', 'unavailable');
CREATE TYPE device_status AS ENUM ('available', 'unavailable');
CREATE TYPE module_status AS ENUM ('available', 'unavailable');

ALTER TABLE gateway ADD COLUMN status gateway_status DEFAULT 'available';
ALTER TABLE device ADD COLUMN status device_status DEFAULT 'available';
ALTER TABLE module ADD COLUMN status module_status DEFAULT 'unavailable';

-- delete all version of this function, some may not exists, but for sure
DROP FUNCTION IF EXISTS get_refresh_module_id(integer);
DROP FUNCTION IF EXISTS get_refresh_module_id(smallint);
DROP FUNCTION IF EXISTS get_refresh_module_id(bigint);

DROP FUNCTION IF EXISTS get_refresh(numeric);
DROP FUNCTION IF EXISTS get_refresh(bigint);
DROP FUNCTION IF EXISTS get_refresh(smallint);
