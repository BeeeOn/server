#!/bin/bash
L_GREEN='\033[1;32m' # Light green
NC='\033[0m' # No Color

printf "${L_GREEN}Load new tasks by ID from task config file!${NC}\n"
pkill -SIGUSR1 baf