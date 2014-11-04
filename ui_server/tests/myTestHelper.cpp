/* 
 * File:   myTestHelper.cpp
 * Author: pavel
 *
 * Created on 28.3.2014, 10:59:04
 */

#include <stdlib.h>
#include <iostream>
#include "../communication.h"
#include "../DBConnector.h"
#include "../Logger.h"
#include "soci-postgresql.h"
#include "soci.h"
#include <stdio.h>
#include <string.h>

using namespace std;
/*
 * Simple C++ Test Suite
 */

string file2String(string path){
std::ifstream ifs((char*)path.c_str());
std::string str( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
return str;
}
bool serverBlackBoxTest(string inFilePath, string outFilePath){
    std::ifstream ifs((char*)inFilePath.c_str());
    std::string inMsg( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    
    std::ifstream ifs2((char*)outFilePath.c_str());
    std::string outMsg( (std::istreambuf_iterator<char>(ifs2) ),
                         (std::istreambuf_iterator<char>()    ) );
    
    string response = resolveMsg((char*)inMsg.c_str(), "ssl", "ip");
    cout<<"ref"<<outMsg<<endl;
    cout<<"my"<<response<<endl;
    
    return response.compare(outMsg) == 0;
}
/*
void test1() {
    std::cout << "myTestHelper test 1" << std::endl;
}

void test2() {
    std::cout << "myTestHelper test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (myTestHelper) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% myTestHelper" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (myTestHelper)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (myTestHelper)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (myTestHelper)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (myTestHelper)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}
*/

