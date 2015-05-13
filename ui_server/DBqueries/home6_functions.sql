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
    
    
    
    
CREATE OR REPLACE FUNCTION devs_set_name() returns trigger
  AS
  $$
    DECLARE
  	def_name text;
  BEGIN
    IF NEW.fk_facilities_mac between 3232235519 AND 3232235776 THEN
      CASE NEW.type 
      
            WHEN x'0B'::integer THEN def_name := 'Stav kotle'  ;
            
            WHEN x'A6'::integer THEN def_name := 'Z1: Typ provozu'  ;
            WHEN x'A7'::integer THEN def_name := 'Z1: Režim provozu'  ;
            WHEN x'A5'::integer THEN def_name := 'Z1: Žádaná teplota'  ;
            WHEN x'0A'::integer THEN def_name := 'Z1: Aktuální teplota'  ;
            
            WHEN x'1A6'::integer THEN def_name := 'Z2: Typ provozu'  ;
            WHEN x'1A7'::integer THEN def_name := 'Z2: Režim provozu'  ;
            WHEN x'1A5'::integer THEN def_name := 'Z2: Žádaná teplota'  ;
            WHEN x'10A'::integer THEN def_name := 'Z2: Aktuální teplota'  ;
            
            WHEN x'2A6'::integer THEN def_name := 'Z3: Typ provozu'  ;
            WHEN x'2A7'::integer THEN def_name := 'Z3: Režim provozu'  ;
            WHEN x'2A5'::integer THEN def_name := 'Z3: Žádaná teplota'  ;
            WHEN x'20A'::integer THEN def_name := 'Z3: Aktuální teplota'  ;
            
            WHEN x'3A6'::integer THEN def_name := 'Z4: Typ provozu'  ;
            WHEN x'3A7'::integer THEN def_name := 'Z4: Režim provozu'  ;
            WHEN x'3A5'::integer THEN def_name := 'Z4: Žádaná teplota'  ;
            WHEN x'30A'::integer THEN def_name := 'Z4: Aktuální teplota'  ;
            
            ELSE def_name := ''   ;
       END CASE;
      UPDATE devices set name = def_name where fk_facilities_mac = NEW.fk_facilities_mac AND type = NEW.type;
      RETURN NEW;
    END IF;  

    
  
  RETURN NEW;
  END;
  
  $$
  LANGUAGE plpgsql;


CREATE TRIGGER set_devs_type
    AFTER INSERT ON devices
    FOR EACH ROW
    EXECUTE PROCEDURE devs_set_name();  
