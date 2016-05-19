#!/bin/bash
L_GREEN='\033[1;32m' # Light green
NC='\033[0m' # No Color

if [ "$1" = "clean" ];
then
	printf "${L_GREEN}Removing BAF object files and binary!${NC}\n"
	make -f ./Makefile_ant2 clean
	printf "${L_GREEN}Removing BAF object files and binary finished!${NC}\n"
else
	printf "${L_GREEN}Compiling BAF binary!${NC}\n"
	make -f ./Makefile_ant2
	printf "${L_GREEN}Compiling BAF binary finished!${NC}\n"
fi