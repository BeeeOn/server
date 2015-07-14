/* 
 * File:   save_custom_writer.h
 * Author: pavel
 *
 * Created on 21. březen 2015, 20:22
 */

#ifndef SAVE_CUSTOM_WRITER_H
#define	SAVE_CUSTOM_WRITER_H
#include "../lib/pugixml.hpp"
#include <string>
#include <iostream>
#include <cstring>

std::string node_to_string(pugi::xml_node node);

#endif	/* SAVE_CUSTOM_WRITER_H */

