-- add new columns
ALTER TABLE module ADD COLUMN not_available BOOLEAN DEFAULT FALSE;
                             
-- change device_euid data type 
ALTER TABLE device ALTER COLUMN device_euid TYPE decimal(20,0);
ALTER TABLE module ALTER COLUMN device_euid TYPE decimal(20,0);
ALTER TABLE log ALTER COLUMN device_euid TYPE decimal(20,0);

-- add new gateway_id columns
ALTER TABLE module ADD COLUMN gateway_id decimal(20,0) DEFAULT NULL;
ALTER TABLE log ADD COLUMN gateway_id decimal(20,0) DEFAULT NULL;
                
-- update new gateway_id columns with data                                 
update module as mod set gateway_id = (select gateway_id from device where device_euid = mod.device_euid );     
update log as l set gateway_id = (select gateway_id from device where device_euid = l.device_euid );                                
           
-- set FK on new gateway_id column         
ALTER TABLE public.device ADD CONSTRAINT device_gateway FOREIGN KEY (gateway_id)
REFERENCES public.gateway (gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

ALTER TABLE public.module ADD CONSTRAINT module_device_fk FOREIGN KEY (device_euid, gateway_id)
REFERENCES public.device (device_euid, gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

ALTER TABLE public.log ADD CONSTRAINT log_module_fk FOREIGN KEY (device_euid, module_id, gateway_id)
REFERENCES public.module (device_euid, module_id, gateway_id) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

-- device PK change
ALTER TABLE device DROP CONSTRAINT facilities_pk CASCADE;
ALTER TABLE device ADD PRIMARY KEY (device_euid,gateway_id); -- device_pkey is created

-- module PK change
ALTER TABLE module DROP CONSTRAINT devices_pk CASCADE;
ALTER TABLE module ADD PRIMARY KEY (device_euid, module_id, gateway_id); --module_pkey is created
                                                          
-- log PK change
ALTER TABLE log DROP CONSTRAINT logs_pk;
ALTER TABLE log ADD PRIMARY KEY (device_euid, module_id, gateway_id, measured_at); --log_pkey is created

-- delete old FK
ALTER TABLE device DROP CONSTRAINT facilities_adapters CASCADE;
ALTER TABLE module DROP CONSTRAINT devices_facilities CASCADE;
ALTER TABLE log DROP CONSTRAINT logs_devices CASCADE;

ALTER TABLE module ALTER COLUMN gateway_id SET NOT NULL;
ALTER TABLE log ALTER COLUMN gateway_id SET NOT NULL;
