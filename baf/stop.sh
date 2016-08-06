#!/bin/bash
L_GREEN='\033[1;32m' # Light green
NC='\033[0m' # No Color

printf "${L_GREEN}Stopping BAF!${NC}\n"
pkill -SIGTERM baf