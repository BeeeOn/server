echo "Starting Framework" 

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./notif
pwd = 'pwd'
export LD_LIBRARY_PATH=$pwd:$LD_LIBRARY_PATH

./framework &
