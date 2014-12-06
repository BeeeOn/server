################################################################
#################### IHA emulator_server ###########################
# Author: Filip Šutovský
# Created: 11/2014

# Requirements
 - Java v1.7.0_##

# Executable:
- !!!! executable file(JAR), folder "lib" and file server.properties MUST be in same folder !!!!
Path: out/artifacts/emulator_server_jar/emulator_server.jar
Linux cmd: java -jar path/to/jar
Windows: double-click on jar file

# Build:
 - emulator_server.properties -> set path to Java JDK
 - Command: ant -buildfile emulator_server.xml