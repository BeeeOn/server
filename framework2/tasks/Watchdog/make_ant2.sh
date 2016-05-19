#!/bin/bash
#!/bin/bash
L_GREEN='\033[1;32m' # Light green
NC='\033[0m' # No Color

if [ "$1" = "clean" ];
then
	printf "${L_GREEN}Removing Watchdog object files and dynamic library!${NC}\n"
	make -f ./Makefile_ant2 clean
	printf "${L_GREEN}Removing Watchdog object files and dynamic library finished!${NC}\n"
else
	printf "${L_GREEN}Compiling Watchdog dynamic library!${NC}\n"
	make -f ./Makefile_ant2
	printf "${L_GREEN}Compiling Watchdog dynamic library finished!${NC}\n"
fi