/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Config.h
 * Author: mrmaidx
 *
 * Created on 14. dubna 2016, 23:08
 */

#ifndef CONFIG_H
#define CONFIG_H

struct Config {

    /** Path to file containing configuration of tasks.*/
    static std::string m_tasks_config_path;
    
    /** Number of working threads user server is going to have. */
    static int m_user_server_threads;
    
    /** Port assigned to user server. */
    static int m_user_server_port;
    
    /** Number of working threads gateway server is going to have. */
    static int m_gateway_server_threads;
    
    /** Port assigned to gateway server. */
    static int m_gateway_server_port;
    
    /** Number of sessins wanted for communication with database. */
    static int m_database_sessions;
    
    /** String formatted to connect to database. */
    static std::string m_database_connection_string;
    
    /** Path to folder with logs. */
    static std::string m_log_folder_path;
    
    /** Level of logging. */
    static std::string m_log_level;
    
    /** Port to ada_server_sender for GatewayInterface. */
    static int m_ada_server_sender_port; 
};

#endif /* CONFIG_H */
