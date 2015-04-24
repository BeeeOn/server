CREATE OR REPLACE FUNCTION upsert_user_returning_uid(d_mail TEXT, d_g_name TEXT, d_f_name TEXT, d_picture TEXT, d_gender TEXT, d_g_id TEXT, d_f_id TEXT) RETURNS integer AS
  $$
  DECLARE
  	t_out integer;
  BEGIN
      LOOP
          -- first try to update the key
          UPDATE users SET  
              given_name = d_g_name, family_name = d_f_name, picture = d_picture,
              gender = d_gender, google_id = d_g_id
          WHERE 
  			CASE  
  				WHEN d_g_id is not NULL then google_id=d_g_id 
  				WHEN d_f_id is not NULL then google_id=d_f_id  
  			END
  		returning user_id into t_out;
  		
          IF found THEN
              RETURN t_out;
          END IF;
          -- not there, so try to insert the key
          -- if someone else inserts the same key concurrently,
          -- we could get a unique-key failure
          BEGIN
              INSERT INTO users(mail, given_name, family_name, picture, gender, google_id, facebook_id) 
                  VALUES (d_mail, d_g_name, d_f_name, d_picture, d_gender, d_g_id, facebook_id) returning user_id into t_out;
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




CREATE OR REPLACE FUNCTION remove_another_superuser() returns trigger
  AS
  $$
  
  BEGIN
    UPDATE users_adapters set role = 'admin' where 
     fk_adapter_id = NEW.fk_adapter_id AND fk_user_id != NEW.fk_user_id;
    
  RETURN NEW;
  END;
  
  $$
  
  LANGUAGE plpgsql;


CREATE TRIGGER only_one_superuser
    BEFORE INSERT OR UPDATE ON users_adapters
    FOR EACH ROW
    WHEN (NEW.role = 'superuser')
    EXECUTE PROCEDURE remove_another_superuser();
    
    



CREATE OR REPLACE FUNCTION def_name_update() returns trigger
  AS
  $$
  
  BEGIN
    UPDATE devices set name = 
  COALESCE( ( select default_name from types where mac_range @> NEW.mac::numeric) ,'') where devices.mac = NEW.mac AND devices.type = NEW.type;
  
  RETURN NEW;
  END;
  
  $$
  
  LANGUAGE plpgsql;


CREATE TRIGGER set_facilities_def_name
    AFTER INSERT ON devices
    FOR EACH ROW
    EXECUTE PROCEDURE def_name_update();  
    
CREATE OR REPLACE FUNCTION facility_type_update() returns trigger
  AS
  $$
  
  BEGIN
    UPDATE facilities set fk_type = NEW.type_id from types where NEW.mac_range <@ facilities.mac::numeric;
  
  RETURN NEW;
  END;
  
  $$
  LANGUAGE plpgsql;


CREATE TRIGGER set_facilities_type
    AFTER INSERT ON types
    FOR EACH ROW
    EXECUTE PROCEDURE facility_type_update();  
     
      
CREATE OR REPLACE FUNCTION check_fac_type() returns trigger
  AS
  $$
  
  BEGIN
    if NEW.fk_type_id = null OR NEW.mac::numeric <@ (select mac_range from types where type_id = NEW.fk_type_id) THEN
      RETURN NEW;
    ELSE  
      RAISE EXCEPTION '% have wrong type, must be null or valid in range table', NEW.mac;
    END IF;  
  END;
  
  $$
  LANGUAGE plpgsql;      
      


CREATE TRIGGER trigger_check_fac_type
    BEFORE INSERT OR UPDATE ON facilities
    FOR EACH ROW
    EXECUTE PROCEDURE check_fac_type();  