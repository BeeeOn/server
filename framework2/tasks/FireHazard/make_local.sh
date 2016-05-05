#!/bin/bash
L_GREEN='\033[1;32m' # Light green
NC='\033[0m' # No Color

if [ "$1" = "clean" ];
then
	printf "${L_GREEN}Removing FireHazard object files and dynamic library!${NC}\n"
	make -f ./Makefile_local clean
	printf "${L_GREEN}Removing FireHazard object files and dynamic library finished!${NC}\n"
else
	printf "${L_GREEN}Compiling FireHazard dynamic library!${NC}\n"
	make -f ./Makefile_local
	printf "${L_GREEN}Compiling FireHazard dynamic library finished!${NC}\n"
fi