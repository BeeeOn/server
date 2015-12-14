
#include <iostream>
#include <string>

#include <soci.h>
#include <postgresql/soci-postgresql.h>

using namespace std;
using namespace soci;

int main(int argc, char *argv[])
{
  
  session sql(postgresql, "port = '5432' dbname = 'home7' user = 'uiserver7' password = '1234' connect_timeout = '3'");
  
  cout << "update devices ... ";
  
  sql << "UPDATE device SET status = CASE measured_at + 3 * get_refresh(gateway_id, device_euid) > extract(epoch from now()) "
                                "WHEN true THEN 'available'::device_status "                                                                                                                                                  
                                "WHEN false THEN 'unavailable'::device_status " 
                                "ELSE 'available'::device_status "
                           "END;"; 
                              
  cout << "done" << endl;
  
  return 0;
}


/*
UPDATE device SET status = CASE measured_at + 3 * get_refresh(gateway_id, device_euid) > extract(epoch from now()) 
                                WHEN true THEN 'available'::device_status                                                                                                                                                  
                                WHEN false THEN 'unavailable'::device_status 
                                ELSE 'available'::device_status
                           END;       
*/