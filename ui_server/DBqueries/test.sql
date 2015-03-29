create table test2(i1 int, i2 int, i3 int, data text);

insert into test2 values (1, null,null,'2'),(null, null,3,'2'),(null, 2,null,'3');

select testF(1,null,null,'x');

CREATE OR REPLACE FUNCTION testF(a int, b int, c int, d text) RETURNS void AS
$$
BEGIN
    update test2 set i1=a,i2=b,i3=c,data = d where CASE WHEN a is not NULL THEN i1=a WHEN b is not NULL then i2=b END;
END;
$$
LANGUAGE plpgsql;

