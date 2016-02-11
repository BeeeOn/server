
CREATE TABLE public.device_parameter(
  gateway_id decimal(20,0) NOT NULL,
  device_euid decimal(20,0) NOT NULL,
  parameter_key varchar(10) NOT NULL,
  parameter_value varchar(255),
	CONSTRAINT device_parameter_pk PRIMARY KEY (gateway_id, device_euid, parameter_key)
);

ALTER TABLE public.device_parameter ADD CONSTRAINT device_parameter FOREIGN KEY (gateway_id, device_euid)
REFERENCES public.device (gateway_id, device_euid) MATCH FULL
ON DELETE CASCADE ON UPDATE NO ACTION;

