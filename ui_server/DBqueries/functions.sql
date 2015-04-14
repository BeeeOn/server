CREATE OR REPLACE FUNCTION upsert_user_returning_uid(d_mail TEXT, d_locale TEXT, d_ver BOOLEAN, d_name TEXT, d_g_name TEXT, d_f_name TEXT, d_link TEXT, d_picture TEXT, d_gender TEXT, d_g_loc TEXT, d_g_id TEXT) RETURNS integer AS
$$
DECLARE
	t_out integer;
BEGIN
    LOOP
        -- first try to update the key
        UPDATE users SET  phone_locale = d_locale, verified_email = d_ver, name = d_name,
                                            given_name = d_g_name, family_name = d_f_name, link = d_link, picture = d_picture,
                                            gender = d_gender, google_locale = d_g_loc, google_id = d_g_id
        WHERE 
			CASE WHEN d_mail is not NULL THEN mail=d_mail  
				WHEN d_g_id is not NULL then google_id=d_g_id
			END
		returning user_id into t_out;
		
        IF found THEN
            RETURN t_out;
        END IF;
        -- not there, so try to insert the key
        -- if someone else inserts the same key concurrently,
        -- we could get a unique-key failure
        BEGIN
            INSERT INTO users(mail, phone_locale, verified_email, name, given_name, family_name, link, picture, gender, google_locale, google_id) 
                VALUES (d_mail, d_locale, d_ver, d_name, d_g_name, d_f_name, d_link, d_picture, d_gender, d_g_loc, d_g_id) returning user_id into t_out;
            RETURN t_out;
        EXCEPTION WHEN unique_violation THEN
            -- Do nothing, and loop to try the UPDATE again.
        END;
    END LOOP;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION upsert_mobile_device(bon_token TEXT, d_mobile_id TEXT,d_type TEXT, d_locale TEXT, d_push_n TEXT, d_uid integer) RETURNS VOID AS
$$
BEGIN
    LOOP
        -- first try to update the key
        UPDATE mobile_devices SET token = bon_token, type = d_type, locale = d_locale, push_notification = d_push_n
        WHERE mobile_id = d_mobile_id and fk_user_id = d_uid;
        IF found THEN
            RETURN;
        END IF;
        -- not there, so try to insert the key
        -- if someone else inserts the same key concurrently,
        -- we could get a unique-key failure
        BEGIN
            INSERT INTO mobile_devices(token, mobile_id, type, locale, push_notification, fk_user_id) 
                VALUES (bon_token, d_mobile_id, d_type, d_locale, d_push_n, d_uid);
            RETURN;
        EXCEPTION WHEN unique_violation THEN
            -- Do nothing, and loop to try the UPDATE again.
        END;
    END LOOP;
END;
$$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION def_name_update() returns trigger
AS
$$

BEGIN
  UPDATE facilities set name = 
COALESCE( ( select default_name from facility_types where mac_range @> NEW.mac::numeric) ,'') where facs.mac = NEW.mac;

RETURN NEW;
END;

$$

LANGUAGE plpgsql;


CREATE TRIGGER set_facilities_def_name
    AFTER INSERT ON facilities
    FOR EACH ROW
    EXECUTE PROCEDURE def_name_update();

