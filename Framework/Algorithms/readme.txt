For compiling Algorithm there is prepared Makefile where is only needed to 
add new target for every algorithm and HERE WE GO...

To test compiling, you have there example Amgorithm: example.cpp, which
can be compiled with command:
  
  make example

Then to run Algorithm application you must load soci using these two commands:

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./notif
pwd = 'pwd'
export LD_LIBRARY_PATH=$pwd:$LD_LIBRARY_PATH

Then try:

./name_of_algorithm -u 1 -a 1 -d 1 -o 1

./watch_and_notify -u 7 -a 1 -d 1136640502157703 -o 70 -v RidOrDevice=device#ID=3976200203#type=10#fval=23.900000#offset=0  -p "3976200203---10#lt#24#notif#mensi 24"

./watch_and_notify -u 25 -a 1 -d 64206 -o 1 -v ID=16964877#type=10#fval=24.900000#offset=0  -p "16964877---10#gt#0#notif#mas tam nad nulou! jede to:)"

./watch_and_notify -u 105794407053259416120 -a 1 -d 1 -o 42 -v ID=16964877#type=10#fval=24.900000#offset=0  -p "16964877---10#gt#0#notif#mas tam nad nulou! jede to:)"

105794407053259416120

WARNING!



TODO:

Catch wrong parameter format! Will cause an error!

 

                                 
                                