#include "DBConnector.h"

using namespace soci;
using namespace std;
#define COM_VERSION "1.0.2"
#define DBTIMEFORMAT "YYYY-MM-DD HH24:MI:SS"
DBConnector::DBConnector()
{
}


DBConnector::~DBConnector(void)
{
    this->clean();
}

DBConnector& DBConnector::getInstance(){
        static DBConnector instance;
        return instance;
}
void DBConnector::setConnectionStringAndOpenSessions(string conString, int sessionPoolSize){
    _connectionString = conString;
       //TODO pozor na deadlock, pokud nekde jsou 2 sql sessions pouzite za sebou
    _poolSize = sessionPoolSize;
    _pool = new connection_pool(_poolSize);

    for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.open(postgresql, _connectionString);
    }
}

void DBConnector::DEBUGsetConnectionStringAndOpenSessions(string conString, int sessionPoolSize){
    _connectionString = conString;
       //TODO pozor na deadlock, pokud nekde jsou 2 sql sessions pouzite za sebou
    _poolSize = sessionPoolSize;
    _pool = new connection_pool(_poolSize);

    for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.open(postgresql, _connectionString);
        sql.begin();
    }
}
void DBConnector::DEBUGrollbackEverything(){
    for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.rollback();
    }
}

string DBConnector::DEBUGexec(string sqlQuery){
    session sql(*_pool);
    string result;
    
    try{
        sql << sqlQuery,into(result);
    }
    catch (soci::postgresql_soci_error& e)
    {
        cerr<<"ERROR in DEBUGexec "<<sqlQuery<<endl;
        cerr<<"cause "<< e.what()<<endl;
    }
    return result;
}

string DBConnector::escapeString(string str) {
    char escaped[300];
    PQescapeString(escaped, str.c_str(), 300);
    return escaped;
}


void DBConnector::clean(){
     for (size_t i = 0; i != _poolSize; ++i)
    {
        session & sql = _pool->at(i);
        sql.close();
    }
     delete _pool;
}





int DBConnector::insertNewUser(string gid, googleInfo gInfo)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"insertNewUser"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                sql << "insert into users( user_id, mail ) values(:gid, :mail)",
                        use(gid, "gid"), use (gInfo.email,"mail");   
                return 1;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                return 0;
        }
}

int DBConnector::insertNewIHAtoken(IhaToken ihaToken, string gId) {
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"insertNewIHAtoken"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                sql << "insert into mobile_devices( token, fk_user_id ) values(:token, :gid)",
                        use (ihaToken,"token"), use(gId, "gid");   
                return 1;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                return 0;
        }
}

GUserId DBConnector::getUserIdbyIhaToken(IhaToken ihaToken) {
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"iha token valid?"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                indicator ind;
                GUserId gUserId = "";
                sql << "select fk_user_id from mobile_devices where token = :token",
                        use (ihaToken,"token"), into(gUserId,ind);   
                return gUserId;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                return 0;
        }
}


bool DBConnector::isUserRegistred(std::string email)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB: is "<<email<<" registred?";
        int c;
        try
        {
                session sql(*_pool);
                sql << "select count(*) from users where mail=:mail;", use(email, "mail"),into(c);
                Logger::getInstance(Logger::DEBUG3)<<email<< c << "\n";
                return c==1; 
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}

int DBConnector::getUserId(std::string email)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB: id of "<<email<<" ?";
        int userId=0;
        try
        {
                session sql(*_pool);
                indicator ind;
                sql << "select user_id from users where mail=:mail;", use(email, "mail"),into(userId, ind);
                sql << "update users set signin_count=signin_count+1 where mail=:mail;", use(email, "mail");
                Logger::getInstance(Logger::DEBUG3)<<email<<"="<< userId<< "\n";
                
                /*if(ind !=i_ok){
                    cout<<"neniOK"<<endl;
                }*/
                return userId; 
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}


string DBConnector::getUserRoleM(GUserId gUserId, string adapterId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB: get role "<<gUserId<<" on "<<adapterId<<endl;
        string role;
        try
        {
                session sql(*_pool);
                sql << "select role from users_adapters where fk_user_id= :guserid and fk_adapter_id = :adapter",
                    use(gUserId, "guserid"), use(adapterId, "adapter"), into(role);
                return role;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
        
}

void DBConnector::updatePhoneLocale(string mail, string phoneLocale){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update locale\n";
        try
        {
                soci::session sql(*_pool);
                std::string xml;
                sql << "update users set phone_locale=:locale  where mail=:mail ;",use(phoneLocale, "locale"),use(mail, "mail");
                
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}
std::string DBConnector::getXMLusersAdapters(std::string gId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"make usersAdapterListXml\n";
        try
        {
                soci::session sql(*_pool);
                std::string xml;
                indicator ind;
                sql << "select xmlagg(xmlelement(name adapter, xmlattributes(adapter_id as id, adapters.name as name, role as role, timezone as utc)))"
                                    "from users join users_adapters on user_id=fk_user_id join adapters on fk_adapter_id = adapter_id where user_id = :gid"
                            ,use(gId,"gid"), soci::into(xml,ind);
                Logger::getInstance(Logger::DEBUG3)<<"sql done result:"<<std::endl;
                if(ind != i_ok)
                    return "";
                return xml;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}

bool DBConnector::isAnyUnregistredAdapter()
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"isAnyUnregistredAdapter"<<"\n";
        int c;
        try
        {
                soci::session sql(*_pool);
                sql << "select count(*) from users_adapters right join adapters on fk_adapter_id=adapter_id where fk_user_id is null;",soci::into(c);
                //std::cout<<"sql done result:"<<c<<std::endl;
                return c>=1;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
        
}

bool DBConnector::isUserParredWithAdapter(string mail, string adapter){
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"isUserParredWithAdapter"<<"\n";
        int c;
        try
        {
                soci::session sql(*_pool);
              
                sql << "select * from users join users_adapters on user_id = fk_user_id join adapters on fk_adapter_id=adapter_id  where users.mail=:mail"
                        " and adapters.adapter_id=:adapter" ,use(mail,"mail"),use(adapter, "adapter"),into(c);

                return (c>0);
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
        return false;
        
}
string DBConnector::getXMLDevicesQueryString(string facilitiesCond)
{
    //TODO inv as involved tam nemá být pokud init = 1
    return " xmlagg("
                        "xmlelement(name  dev,xmlattributes(init as init, mac as did, fk_room_id as lid, refresh as refresh, battery as batt,"
                                                " timestamp as time, involved  as inv, quality as rssi),"
                                                 "(select xmlagg(xmlelement(name part,xmlattributes(type as type, visibility as vis, name as name, value as val)))from devices where fk_facilities_mac=mac "+facilitiesCond+")"
                    ")"
                ") "
                ;

}
std::string DBConnector::getXMLAllDevs(string adapter)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getCommunicationXml"<<"\n";
        
        try
        {
            soci::session sql(*_pool);
            
            std::string xml;
            indicator ind;
            
            sql << "select " +
                                           getXMLDevicesQueryString() +
                    "from  adapters  join facilities on adapters.adapter_id=facilities.fk_adapter_id"
                    " where adapters.adapter_id=:adapter and init!='0' " 
                    ,use(adapter,"adapter"),soci::into(xml,ind);
            

        if(ind != i_ok){
                Logger::getInstance(Logger::DEBUG3)<<"query not ok"<<"\n";
               return "";         
            }
            return xml;
        }
        catch (soci::postgresql_soci_error& e)
        {
            Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
            throw;
        }
        
}

string DBConnector::getXMLdevices(GUserId userId, vector<string> adaptersVec, vector<device> devicesVec)
{
         Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
        try
        {
                soci::session sql(*_pool);
                Logger::getInstance(Logger::DEBUG3)<<"devices n."<<devicesVec.size()<<"\n";
                if( devicesVec.size() == 0)
                    return "";
                
                stringstream ssD,ssF,ssA;
                ssD<<"(";
                 ssF<<"(";
                for (unsigned int i=0; i<devicesVec.size(); i++){
                    ssD << "('"<<devicesVec[i].id<<"',"<<devicesVec[i].type<<")";
                    ssF << "'"<<devicesVec[i].id<<"'";
                    
                    if( i != devicesVec.size()-1){
                        ssD <<",";
                        ssF <<",";
                    }
                }
                ssD<<")";
                 ssF<<")";
                 
                 ssA<<"(";
                for (unsigned int i=0; i<adaptersVec.size(); i++){
                    ssA << "'"<<adaptersVec[i]<<"'";
                    
                    if( i != adaptersVec.size()-1){
                        ssA <<",";
                    }
                }
                ssA<<")";
                string devices = ssD.str();
                string facs = ssF.str();
                string adapters = ssA.str();
              
                Logger::getInstance(Logger::DEBUG3)<<"check ada string::"<<adapters<<endl;
                Logger::getInstance(Logger::DEBUG3)<<"check devices string:"<<devices<<endl;
                Logger::getInstance(Logger::DEBUG3)<<"check facilities string::"<<facs<<endl;
                
                string xml;
                indicator ind;
                //cout<<getXMLDevicesQueryString(" and concat(fk_facilities_mac,type) in "+devices);
                statement st = (sql.prepare <<
                        "select  xmlagg("
		"xmlelement(name adapter, xmlattributes(adapter_id as id),"
					    "(select  "
						    "xmlagg(xmlelement(name  dev,xmlattributes(mac as did, fk_room_id as lid, refresh as refresh, battery as batt,"
                                                                                                                                     " timestamp as time, involved  as inv, quality as rssi),"
								"(select xmlagg(xmlelement(name part,xmlattributes(type as type, visibility as vis, name as name,value as val))) from devices where "
									"(host(fk_facilities_mac) , type) in "+devices+" "
									"and devices.fk_facilities_mac = facilities.mac"
								")"								
							   ")"
						   " ) from facilities where facilities.mac in "+facs +" and"
						   " adapters.adapter_id=facilities.fk_adapter_id"
							
						")"
		")"
	") "
"from adapters where adapter_id in "+adapters
                        ,
                        soci::into(xml, ind) );
                //TODO napojeni na uživatlee
                st.execute(true);
                return xml;
                /*
              
                string searchedString;
                string errText;
                
                for (unsigned int i=0; i<devices.size(); i++){
                    
                    searchedString = devices[i].id + "\" " + "type=\"" + devices[i].type;
                    if(xml.find( searchedString ) ==std::string::npos)
                        errText += "<device id=\"" + devices[i].id + "\" type=\"" + devices[i].type + "\" />";
                }
 * //TODO pozdeji udělat kontroly
 *  //TODO castecny neuspech znamena false
  */
               /*
                if(errText == "")
                    return xml;
                else
                    throw ServerException(ServerException::DEVICE_ID_TYPE, errText);
                */
        }
        catch (postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}


string DBConnector::getXMLNewDevices(string adapterId)
{
         Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                
                string xml;
                indicator ind;
//TODO možná tady nemusi byt join na devices
                statement st = (sql.prepare <<"select " +
                        getXMLDevicesQueryString() +
                       "from adapters join facilities on adapter_id=fk_adapter_id join devices on mac=fk_facilities_mac where init='0' and adapter_id =:adapter" 
                        ,use(adapterId, "adapter"),soci::into(xml, ind) );
                
                st.execute(true);
                             
                return xml;
                
        }
        catch (postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}


bool DBConnector::isAdapterRegistrable(string adapterId){
        
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"isAdapterRegistrable"<<"\n";
        int c=0;
        try
        {
                soci::session sql(*_pool);
                
                sql << "select registrable from adapters where adapter_id = :adapter"
                        ,use(adapterId,"adapter"),soci::into(c);
                Logger::getInstance(Logger::DEBUG3)<<"sql done result:"<<c<<std::endl;
                return c;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}




string DBConnector::getUserId(session & sql, string userMail)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getUserId"<<"\n";
        try
        {
                string userId;
                sql << "select user_id from users where mail=:u_mail"
                        ,use(userMail,"u_mail"),soci::into(userId);
                return userId;
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}


int DBConnector::parAdapterWithUserIfPossible(int adapterId, string adapterName, string gId){
    Logger::getInstance(Logger::DEBUG3)<< "par user - adapter (new user?)\n";
    try
    {
        soci::session sql(*_pool);
       
        string role = "superuser";
        
        sql << "insert into users_adapters(fk_adapter_id, fk_user_id, role) values(:a_id , :gId, :role)",
                use(adapterId, "a_id"), use(role, "role"),  use(gId, "gId");
        sql << "update adapters set name=:a_name where adapter_id=:a_id",
                 use(adapterId, "a_id"), use(adapterName, "a_name");
        //TODO if adapter is registrable, 2x sql
        Logger::getInstance(Logger::DEBUG3)<< "parred\n";
        return 1;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

void DBConnector::insertNewUserParredWithUnregistredAdapter(string adapterId, string mail, bool registerNewUser)
{
    Logger::getInstance(Logger::DEBUG3)<< "join user - adapter (new user?)\n";
    
    try
    {
        soci::session sql(*_pool);
        if(registerNewUser){
            Logger::getInstance(Logger::DEBUG3)<< "register user\n";
            sql << "insert into users( mail ) SELECT :mail "
                                "WHERE NOT EXISTS ("
                                "SELECT * FROM users WHERE mail=:mail "
                                ")",
                    use(mail, "mail");   
        } 
        
        string role = "superuser";
        
        sql << "insert into users_adapters(fk_adapter_id, fk_user_id, role) values(:a_id, (select user_id from users where mail=:u_mail), :role)",
                use(adapterId, "a_id"), use(role, "role"),  use(mail, "u_mail");
        Logger::getInstance(Logger::DEBUG3)<< "inserted\n";
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
    
}

int DBConnector::insertNewRoom(string adapterId, string roomType, string roomName)
{
    Logger::getInstance(Logger::DEBUG3)<< "insert new room\n";
    
    try
    {
        soci::session sql(*_pool);
        //TODO zamknout row, zmenit id na DB typ serial?
       int newRoomId;
       /*  sql << "select max(room_id) from rooms where fk_adapter_id = "+adapterId, soci::into(newRoomId);
        newRoomId++;*/
        sql << "insert into rooms (fk_adapter_id, type, name) values( :a_id, :type, :name) returning room_id",
                use(adapterId, "a_id"), use(roomType, "type"), use(roomName, "name"), into(newRoomId);
    
        Logger::getInstance(Logger::DEBUG3)<< "inserted\n";
        return newRoomId;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }   
}

void DBConnector::updateRoom(string adapterId, string roomId, string type, string name){
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "update room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        statement st = (sql.prepare <<  "update rooms set type=:type , name=:name where room_id=:id and fk_adapter_id= :a_id;",
                use(type, "type"),use(name, "name"), use(roomId, "id"), use(adapterId, "a_id") );
        st.execute(true);
        
        int updateCount = st.get_affected_rows();
        Logger::getInstance(Logger::DEBUG3)<<"update"<<updateCount<<endl;
        if(updateCount == 0)
            throw ServerException(ServerException::MISSING_ENTITY);
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }  
}
void DBConnector::deleteRoom(string adapterId, string roomId){
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "delete room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        sql << "delete from rooms where room_id=:id and fk_adapter_id=:a_id ",
                use(roomId, "id"), use(adapterId, "a_id");
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }  
}

int DBConnector::updateFacility(string adapterId, string id, string init, string locationId, string refresh) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update Facility"<<id<<"\n";
    try
    {       
        soci::session sql(*_pool);
        string query = "update facilities set";
        if(init != "")query.append(" init='"+escapeString(init)+"', ");
        if(locationId != "")query.append(" fk_room_id='"+escapeString(locationId)+"', ");
        if(refresh != "")query.append(" refresh='"+escapeString(refresh)+"', ");
        
        query = query.substr(0,query.size()-2);//erase last ', '
        query.append(" where mac ='"+escapeString(id)+"' ");
        //and fk_adapter_id='"+escapeString(adapterId)+"'");
        
        statement st = (sql.prepare << query);
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DBConnector::updateDevice(string adapterId, string id, string type, string name, string visibility)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"updateDevice"<<id<<"\n";
        try
        {       
                soci::session sql(*_pool);
                string query  = "update devices set";
                if(name != "")query.append(" name='"+escapeString(name)+"', ");
                if(visibility != "")query.append(" visibility='"+escapeString(visibility)+"', ");
                
                query = query.substr(0,query.size()-2);//erase last ', '
                
                query.append(" where fk_facilities_mac ='"+escapeString(id)+"' and type='"+escapeString(type)+"' ");
                //and fk_adapter_id='"+escapeString(adapterId)+"'");
                
                Logger::getInstance(Logger::DEBUG3)<<"SQL partial update: "<<query<<"\n";
                
                statement st = (sql.prepare << query);
                st.execute(true);
                
                return st.get_affected_rows();
                                            
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                return 0;
        }
}

string DBConnector::getXMLrooms(string adapterId)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"roomsList (adapter="<<adapterId<<")\n";
    try{
            soci::session sql(*_pool);

            string xml;
            indicator ind;
            sql<<"select xmlagg("
                    "xmlelement(name loc , "
                        "xmlattributes(room_id as id, type as type, name as name) "
                    ")" 
            ")"
            "from rooms where fk_adapter_id =:adapter and room_id is not null" 
            ,use(adapterId,"adapter"),soci::into(xml, ind);
            //cout<<"xml"<<xml<<endl;
            if(ind != i_ok)
                    return "";
            return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
            throw;
    }
}

string DBConnector::getXMLDeviceLog(string adapterId, device device, string logFrom, string logTo, string dataType, string interval)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"log (dev="<<device.id<<")\n";
    
    try{
            soci::session sql(*_pool);

            string timeCond;
            
            if(logFrom == "")
                logFrom = "0";
            if(logTo == "")
                logTo = "9223372036854775807";//max bigint
            
            string aggregationFunction;
            if(dataType == "avg")
                aggregationFunction = "avg";
            else if(dataType == "min")
                aggregationFunction = "min";
            else if(dataType == "max")
                aggregationFunction = "max";
            else
                throw ServerException(ServerException::AGG_FUNCTION);
            
            timeCond = " logs.timestamp between "+escapeString(logFrom)+" and "+escapeString(logTo)+" ";

            string DBIntervalString;
             int intInterval;
            try{
                intInterval = atoi( interval.c_str() );
            }
            catch(...){
                intInterval=0;
            }
            string xml;
            indicator ind;
            if( intInterval==0){ //RAW output
                Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"RAW data\n";
                
                sql<<"select xmlagg("
                        " xmlelement(name row, timestamp || ' ' || logs.value )"
                        ")"
                        "from logs where  fk_facilities_mac= :d_id and  fk_devices_type= :d_type and"+ timeCond
                        ,use(device.id,"d_id"), use(device.type, "d_type"), soci::into(xml, ind);
            }else{
                Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"data by "<< DBIntervalString <<" and" << timeCond <<"\n";
                
                //cout<<"SELECT * FROM generate_series('"+logFrom+"'::timestamp,'"+logTo+"', '1 "+DBIntervalString+"') as full_time; "<<endl;
                //cout<<"select date_trunc('"+DBIntervalString+"',max(timestamp) ), avg(value) from logs where fk_devices_mac= '"+device.id+"' and  fk_devices_type= '"+device.type+"'"+ "group by date_trunc('"+DBIntervalString+"',timestamp )  ;"<<endl;
                ostringstream oss ;
               /* oss<<"select xmlagg( xmlelement(name row, to_char(full_time,xxxxxxxx) ,' ' ,round( coalesce(avg, 'NaN') , 2 )) ) from"
                        "(select * from generate_series('"+logFrom+"'::timestamp,'"+logTo+"', '1 "+DBIntervalString+"') as full_time) t1 "
                        "left join "
                        "(select date_trunc('"+DBIntervalString+"',max(logs.timestamp) ), "+aggregationFunction+"(logs.value) from logs where logs.fk_facilities_mac=:d_id and  fk_devices_type= :d_type "
                        "and "+timeCond+//" and (select mac from devices where fk_adapter_id=:a_id and mac=:d_id and type=:d_type) is not null "
                        "group by date_trunc('"+DBIntervalString+"',logs.timestamp )  order by date_trunc('"+DBIntervalString+"',max(logs.timestamp))  ) t2 "
                        "on full_time=date_trunc";*/
                 //TODO nezavisi na adapter_id        
                
                oss<<"select xmlagg( xmlelement(name row,a,' ',b ))from (select ceil(avg(timestamp)) as a,  trunc("<<aggregationFunction<<"(value)::numeric,2) as b from "
                        "logs where  fk_facilities_mac= :d_id and  fk_devices_type= :d_type and"<< timeCond<<" group by timestamp/"<<intInterval<<") as innerQ";
                        //select xmlagg(xmlelement(name row,a,' ',b ))from (select ceil(avg(timestamp)) as a,min(value) as b from logs where timestamp between 0 and 10000000000 group by timestamp/2) as x;
                string query;
                query= oss.str();
                Logger::getInstance(Logger::DEBUG3)<<query<<endl;
                sql << query,use(adapterId,"a_id"), use(device.id,"d_id"), use(device.type, "d_type"), soci::into(xml, ind);
                
            }
            
            if(ind != i_ok)
                return "";
            return xml;
            
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

void DBConnector::updateAdaptersTimezone(string adapterId,  string newTimeZone){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update time zone (adapter="<<adapterId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        sql <<"update adapters set timezone = :timezone where adapter_id = :adapter"
                ,use(newTimeZone, "timezone"),use(adapterId,"adapter");             
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::getTimeZone(string adapterId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get time zone (adapter="<<adapterId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string timezone;
        indicator ind;
        sql << "select timezone from adapters where adapter_id=:adapter"
                ,use(adapterId,"adapter"), into(timezone, ind);
        
            if(ind != i_ok)
                throw ServerException(ServerException::ADAPTER_ID);
        return timezone;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::getXMLconAccounts(string adapterId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"conAccountList (adapter="<<adapterId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        indicator ind;
        sql << "select xmlagg ("
                                                "xmlelement(name user, xmlattributes(mail as mail, role as role, given_name as name, family_name as surname , gender as gender))"
                                                ")"
                "from users left join users_adapters on user_id = fk_user_id where fk_adapter_id = :adapter"
                ,use(adapterId,"adapter"), into(xml, ind);
        
            if(ind != i_ok)
                return "";
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DBConnector::delConAccount(string adapterId, string userMail){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"del acc (adapter="<<adapterId<<" acc:"<<userMail<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st=(sql.prepare << "delete from users_adapters where fk_user_id=(select user_id from users where mail=:mail) and fk_adapter_id=:adapter"
                ,use(adapterId,"adapter"), use(userMail,"mail") );
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

int DBConnector::changeConAccount(string adapterId, string userMail, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (adapter="<<adapterId<<" acc:"<<userMail<<" role:"<<newRole<<")\n";
    try{
        soci::session sql(*_pool);
        statement st = (sql.prepare << " update users_adapters set role=:role  where fk_user_id=(select user_id from users where mail=:mail) and fk_adapter_id=:adapter "
                "and check_downgrade_last_superuser(:adapter, (select user_id from users where mail=:mail))"
                ,use(newRole, "role"),use(adapterId,"adapter"), use(userMail,"mail"));
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}
int DBConnector::addConAccount(string adapterId, string userMail, string newRole){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"change acc (adapter="<<adapterId<<" acc:"<<userMail<<" role:"<<newRole<<")\n";
    
    try{
        soci::session sql(*_pool);
        
        sql << "LOCK TABLE users IN SHARE ROW EXCLUSIVE MODE";
        
        sql << "insert into users( mail ) SELECT :mail "
	"WHERE NOT EXISTS ("
		"SELECT * FROM users WHERE mail=:mail "
	")",
                        use(userMail, "mail");   
        
         statement st = (sql.prepare << "insert into users_adapters(fk_adapter_id, fk_user_id, role) values(:a_id, (select user_id from users where mail=:mail), :role)",
                use(adapterId, "a_id"), use(userMail, "mail"), use(newRole, "role") );
         st.execute(true);
         return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}

/***************************************************************************************************************/
 /*                                                           VIEWS                                                                                                              */
/***************************************************************************************************************/
void DBConnector::addView(string gId, string viewName, string viewIcon){
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"add view:"<<viewName<<")\n";
    try{
        soci::session sql(*_pool);
       
        sql << "insert into views( name, fk_user_id, icon ) values(:name,  :gid, :icon)",
                        use(viewName, "name"), use(gId, "gid"), use(viewIcon, "icon");        
        return ;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
} 

void DBConnector::addDeviceToView(string  viewName, string  gId, string deviceId, string deviceType){
            Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"addDeviceToView:"<<viewName<<")\n";
    try{
        soci::session sql(*_pool);
       //TODO musi se propojit s mailem-adapterem-device, protože jinak by mohl koukat na cizi zařizeni
        sql << "insert into views_devices ( fk_view_name, fk_user_id, fk_devices_mac, fk_devices_type ) values(:name,  :gid, :d_id, :d_type)",
                        use(viewName, "name"), use(gId, "gid"), use(deviceId, "d_id"), use(deviceType, "d_type");        
        return ;
    } 
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

void DBConnector::removeDeviceFromView(string  viewName, string  gId, string deviceId, string deviceType){
            Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"removeDeviceToView:"<<viewName<<")\n";
    try{
        soci::session sql(*_pool);
        
        sql << "delete from views_devices where fk_view_name=:name and fk_user_id = :gid and fk_devices_mac = :d_id and fk_devices_type = :d_type",
                        use(viewName, "name"), use(gId, "gid"), use(deviceId, "d_id"), use(deviceType, "d_type");        
        return ;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

void DBConnector::delView(string viewName, string gId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"delView:"<<viewName<<")\n";
    try{
        soci::session sql(*_pool);
        
        sql << "delete from views where name=:name and fk_user_id = :gid ",
                use(viewName, "name"), use(gId, "gid");        
        return ;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::viewsList(string gId){
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"viewList:"<<gId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        indicator ind;
        sql << "select xmlagg("
                                                "xmlelement(name \"view\", xmlattributes(name as name, icon as icon)) "
                                                  ")"
                       "from views where fk_user_id=:gid",
            use(gId, "gid"),into(xml, ind);

            if(ind != i_ok)
                return "";        
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::insertNewCondition(string gId, string condName, string condType, string condXml) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"insert Cond:"<<condName<<"\n";
    try{
        soci::session sql(*_pool);
        
        string newCondId;
        sql << "insert into conditions(name, type, xml, fk_user_id) values(:name, :type, :xml,:gid ) returning cond_id",
                use(condName, "name"),use(condType,"type"),use(condXml,"xml"),use(gId,"gid"),into(newCondId);

        return newCondId;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::getCondition(string condId) {
//TODO možnost utoku na ID, nutno propojit s user (select user_id from users where mail=:mail)
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get Cond:"<<condId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        sql << "select xml from conditions where cond_id=:c_id",
                use(condId, "c_id"), into(xml);
        if(xml=="")
            throw ServerException(ServerException::WRONG_COND_OR_EMTPY_COND);
        
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::getConditionsList(string mail) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get conds list for "<<mail<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        indicator ind;
               sql << "select xmlagg("
                                                        "xmlelement(name \"conditions\", "
                                                            "xmlelement(name \"condition\", xmlattributes(cond_id as id, name as name, type as type)) "
                                                         ")"
                                                  ")"
                       "from conditions where fk_user_id=(select user_id from users where mail=:mail)",
                use(mail, "mail"), into(xml,ind);
        if(xml=="")
            throw ServerException(ServerException::WRONG_COND_OR_EMTPY_COND);
        
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DBConnector::updateCondition(string condId, string condName, string condType, string condXml) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update cond "<<condId<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << "update conditions set type=:type, name=:name, xml=:xml where cond_id=:id ",
                use(condId, "id"),use(condName, "type"),use(condType, "name"),use(condXml, "xml")
             );
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

void DBConnector::deleteCondition(string condId) {
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"delete cond "<<condId<<")\n";
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << "delete from conditions where cond_id=:id",
                use(condId, "id")
             );
        st.execute(true);
        return ;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::insertNewAction(GUserId userId, string actionName, string actionXml) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"insert action:"<<actionName<<"  "<<actionXml<<"\n";
    try{
        soci::session sql(*_pool);
        
        string newActionId;
        sql << "insert into actions(name, xml, fk_user_id) values(:name, :xml, :u_id) returning action_id",
                use(actionName, "name"),use(actionXml,"xml"),use(userId,"u_id"),into(newActionId);

        return newActionId;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::getActionsList(GUserId userId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get actions list for "<<userId<<")\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        indicator ind;
               sql << "select xmlagg("
                                                        "xmlelement(name \"actions\", "
                                                            "xmlelement(name \"complexaction\", xmlattributes(action_id as id, name as name)) "
                                                         ")"
                                                  ")"
                       "from actions where fk_user_id=:u_id",
                use(userId, "u_id"), into(xml,ind);
        if(xml=="")
            throw ServerException(ServerException::WRONG_OR_EMPTY_ACTION);
        
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DBConnector::getAction(string actionId) {
    //TODO možnost utoku na ID, nutno propojit s user 
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"get action:"<<actionId<<"\n";
    try{
        soci::session sql(*_pool);
        
        string xml;
        sql << "select xml from actions where action_id=:a_id",
                use(actionId, "a_id"), into(xml);
        if(xml=="")
            throw ServerException(ServerException::WRONG_OR_EMPTY_ACTION);
        
        return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DBConnector::updateAction(string actionId, string actionName, string actionXml) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update action "<<actionName<<"\n";
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << "update actions set name=:name, xml=:xml where action_id=:id",
                use(actionId, "id"), use(actionName, "name"), use(actionId, "xml")
             );
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

void DBConnector::deleteAction(string actionId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"delete action "<<actionId<<"\n";
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << "delete from actions where action_id=:id",
                use(actionId, "id")
             );
        st.execute(true);
        return ;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DBConnector::connectConditionWithAction(string condId, string actionId) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"condition + action"<<"\n";
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << "update conditions set fk_action_id=:a_id where cond_id=:c_id",
                use(actionId, "a_id"),use(condId, "c_id")
             );
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

int DBConnector::updateUsersGCMID(GUserId userId, string gcmid) {
    return 0;
}

int DBConnector::updateUserGoogleInformation(GUserId userId, googleInfo gInfo) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update ginfo"<<endl;
    try{
        soci::session sql(*_pool);
        
        statement st = (sql.prepare << "update users set "
                "given_name = :given_name, family_name = :family_name, gender = :gender, google_id =:g_id, "
                "link = :link, google_locale = :g_locale, picture = :g_pict "//, verified_email = :ver_mail "
                "where user_id = :user_id",
                use(gInfo.given_name, "given_name"),use(gInfo.family_name, "family_name"),use(gInfo.gender, "gender"), use(gInfo.id, "g_id"),
                use(gInfo.link,"link"), use(gInfo.locale,"g_locale"), use(gInfo.picture,"g_pict"),//use(gInfo....., "ver_mail"),
                use(userId, "user_id")
             );
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}
