/*
 * File:   newtestclass.cpp
 * Author: pavel
 *
 * Created on 5.9.2014, 11:57:32
 */

#include "newtestclass.h"
#include "MsgInAdapterListen.h"
#include "Config.h"

using namespace soci;
CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass);

newtestclass::newtestclass() {
}

newtestclass::~newtestclass() {
}

void newtestclass::setUp() {
   DBConnector::getInstance().DEBUGsetConnectionStringAndOpenSessions("port = '5432' dbname = 'home2' user = 'pavel'");
    Logger::getInstance().setVerbose(Logger::NO_OUTPUT);
     ComTable::getInstance().setComInfo("user2@gmail.com");
    ComTable::getInstance().setComInfo("user1@gmail.com");

}

void newtestclass::tearDown() {
    DBConnector::getInstance().DEBUGrollbackEverything();
}

    /*
    ThreadPool pool(10);
// enqueue and store future
auto result = pool.enqueue([](int answer) { return answer; }, 42);

// get result from future
std::cout << result.get() << std::endl;
    return 0;*/
   
        //isGTokenOk("ya29.1.AADtN_XB7Hgh5lCXyC5OKnIU_k1golUWsS9C_yXj9OBiw40RqFgGBp6R51JekZPG9Y5GGQ", "");
        //return 0;
  /*  SocketClient sc(12345,"localhost");
    sc.write("ahoj");
    cout<<sc.read();
    return 0;*/
   /* session sql;
    long int val;
    string table ="users.id";
    sql.open(postgresql,Config::getInstance().getDBConnectionString());
    //sql.begin();
    //cout<< sql.get_next_sequence_value("users_user_id_seq", val);
    sql<< "insert into users( mail ) values('novy5')";
    
    
    
    
    cout<<sql.get_last_insert_id(table, val);
    cout<<endl;
    cout<<"xyxcy"<<val<<endl;
    //sql.rollback();*/
  /*  if(1){
        cout<<"start socket test"<<endl;
        SocketClient sc(9091);
        
        cout<<"read"<<endl;
        sc.write((string)"soc");
        cout<<"write"<<endl;
        sc.read();
        cout<<"soc Test end"<<endl;
        return 0;
    }
    */

void newtestclass::testMethod() {
    return ;
    soci::session sql;
    sql.open(postgresql,Config::getInstance().getDBConnectionString());
    string xml;
        //statement st = (sql.prepare << " select xmlelement(name a, xmlattributes(name as name) ) from adapters ",into(xml)
    statement st = (sql.prepare << " select adapter_id  from adapters where adapter_id=-1 limit 1",into(xml)
             );
        st.execute(true);
        cout<<xml<<"Asdasd"<<st.get_affected_rows()<<endl;
    /*
    soci::session sql;
    sql.open(postgresql,Config::getInstance().getDBConnectionString());
    
        statement st = (sql.prepare << "update rooms set room_id=1111 where room_id='6535' "
             );
        st.execute(true);
        cout<<"Asdasd"<<st.get_affected_rows()<<endl;
        */
        
    CPPUNIT_ASSERT(true);
    return ;
    
    /*
    int poolSize = 1;
    connection_pool *pool = new connection_pool(poolSize);

    for (size_t i = 0; i != poolSize; ++i)
    {
        session & sql = pool->at(i);
        sql.open(postgresql, "port = '5432' dbname = 'starthome1' user = 'pavel'");
    }
    
    session sql(*pool);
    sql.begin();
    try{
        sql<< "insert into users( mail ) values('novy10')";
        //sql <<"insert into nothing( no ) values('no')";
    }catch(...){
        CPPUNIT_ASSERT_MESSAGE("catched",false);
    }
    sql.commit();
    CPPUNIT_ASSERT(true);*/
}

void newtestclass::testFailedMethod() {
    CPPUNIT_ASSERT(true);
    return ;
    /*
    char* msg="<?xml version=\"1.0\" encoding=\"UTF-8\"?><communication version=\"1.0\" id=\"1\" state=\"adapterlisten\" adapter=\"64206\"></communication>";
    pugi::xml_document* doc = new pugi::xml_document();
    doc->load(msg);
    
    //MsgInAdapterListen foo(msg,doc);
    
    
    cout<<typeid(MsgInAdapterListen).name()<<endl;
    cout<<typeid(MsgInAdapterListen() ).name()<<endl;
   // cout<<typeid(foo).name()<<endl;
    
    cout<<endl;
    cout<<MsgInAdapterListen::state<<endl;
    MsgInAdapterListen* a = new MsgInAdapterListen(msg,doc);
    IMsgInLoginAndAdapterAccessRequired* b = new MsgInAdapterListen(msg,doc);
    
    cout<< a->state<<endl;
    cout<<"xx"<<endl;
    */
    CPPUNIT_ASSERT(true);
}

