For compiling Algorithm there is prepared Makefile where is only needed to 
add new target for every algorithm and HERE WE GO...

Then to run Algorithm application you must load soci using these two commands:

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./notif

Then try:

./name_of_algorithm -u 1 -a 1 -d 1 -o 1 -v ID=1#type=1#bval=true#offset=1$ID=2#type=1#bval=true#offset=1  -p param1#param2#...

WARNING!

TODO:

Catch wrong parameter format! Will cause an error!

 

                                 
                                