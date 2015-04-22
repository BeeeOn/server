How to start framework
-----------------------
To compile: make

Then to run Algorithm application you must load soci using these two commands:

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./notif
pwd = 'pwd'
export LD_LIBRARY_PATH=$pwd:$LD_LIBRARY_PATH

Run:
./framework &

Stop:
pkill -SIGTERM framework

ResetAlgorithms:
pkill -SIGINT framework

-----------------------------------------------------------

If you will have errors like these:
          Probl-m s pojmenov-n-m soketu.
Probl-m s p-ijet-m spojeni

Then you have to kill process listening at ports that framework listens to:
7082
7083
7084

Find these processes with this command: fuser 7083/tcp
and: kill <pid>