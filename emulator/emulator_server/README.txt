################################################################
#################### IHA emulator_server ###########################
# Author: Filip Šutovský
# Created: 11/2014

# Requirements
 - Java v1.7.0_##

# Executable:
- !!!! executable file(JAR), folder "lib" and file server.properties MUST be in same folder !!!!
Path: out/artifacts/emulator_server_jar/emulator_server.jar
Linux cmd: java -jar path/to/jar [properties/file/path]

# Build:
 - emulator_server.properties -> set path to Java JDK
 - Command: ant -buildfile emulator_server.xml

#Service
- !! log in as ROOT!!
- service start
    service eserver start
- service stop
    service eserver stop
- service restart
    service eserver restart

- log location
    /home/xsutov00/emulator_server/logs/es_log********.log
- changing log location
    change "fileName" in file /resources/log4j2.xml, build application and restart service