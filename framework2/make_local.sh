#!/bin/bash

#Black        0;30     Dark Gray     1;30
#Red          0;31     Light Red     1;31
#Green        0;32     Light Green   1;32
#Brown/Orange 0;33     Yellow        1;33
#Blue         0;34     Light Blue    1;34
#Purple       0;35     Light Purple  1;35
#Cyan         0;36     Light Cyan    1;36
#Light Gray   0;37     White         1;37
RED='\033[0;31m'
L_RED='\033[1;31m' # Light red
GREEN='\033[0;32m'
L_GREEN='\033[1;32m' # Light green
NC='\033[0m' # No Color

if [ "$1" = "clean" ];
then
	printf "${L_GREEN}Removing BAF object files!${NC}\n"
	make -f ./Makefile_local clean
	printf "${L_GREEN}Removing BAF object files finished!${NC}\n"
elif [ "$1" = "delete" ];
then
	printf "${L_GREEN}Removing BAF binary!${NC}\n"
	rm ./baf
	printf "${L_GREEN}Removing BAF binary finished!${NC}\n"
else
	printf "${L_GREEN}Compiling BAF binary!${NC}\n"
	make -j 4 -f ./Makefile_local
	printf "${L_GREEN}Compiling BAF binary finished!${NC}\n"
fi