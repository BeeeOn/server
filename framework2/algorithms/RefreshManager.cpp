/* 
 * File:   RefreshManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "RefreshManager.h"

RefreshManager::RefreshManager(int id, MANAGER_TYPE type, std::string name) :
    TimedAlgorithmManager(id, type, name)
{
}
/*
RefreshManager::RefreshManager(const RefreshManager& orig) {
}
*/
RefreshManager::~RefreshManager() {
}

