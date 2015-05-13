Framework(R) Version 1.0 2/5/2015


How to build framework
-----------------------
1)Download Notification and build it (make in the Notificator directory)
2)Copy libnotif.so to root directory of Framework and to the directory of Algorithms
3)In directory Algorithms do make. It will build all algorithms
3)Compile framework (make)   

How to start framework
-----------------------
1)To run application you must load soci using these commands:

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./notif
pwd = 'pwd'
export LD_LIBRARY_PATH=$pwd:$LD_LIBRARY_PATH

2)Run (with super user privilegies):
./framework &

How ro stop framework
-----------------------
pkill -SIGTERM framework

Reseting Algorithms during run of the service
-----------------------
pkill -SIGINT framework

Prepared scripts
-----------------------
Before using, needed to set rights (eg. chmod a+x startFramework.sh)

Start:
./startFramework.sh

Stop:
./stopFramework.sh

Reload Algorithms:
./reloadFramework.sh

Daemon prepared for init.d:
./daemon restart

Configuration files
-----------------------
When default, everything will work correctly.

1) Configuration of framework - config.xml

<?xml version="1.0" encoding="UTF-8"?>
<framework_config
	portUIServer="7082"
	portAdaRecieverServer="7083"
	portAdaSenderServer="7081"
	portAlgorithmsServer="7084"
	DBName="home5"
	maxNumDBConnections="30"
	recieveBuffSize="1000"
	algorithmsConfig="Algorithms/AlgsConfig.xml" ... path to Algorithm config file
>
	<loger
		fileName="log"       ... prefix for name of log files
		appName="FRAMEWORK"  ... name of application used in lines of logs
		verbosity="7"        ... the depth of logging
		filesCnt="5"         ... maximum number of files to create during logging
		LinesCnt="100"       ... maximum number of lines per file
	/>
</framework_config>

2) Configuration of Algorithms - default Algorithms/algsConfig.xml
This config is essential for enabling algorithms for Framework. It can be edited
and reloaded during run of the application! (to reload you can script)


<?xml version="1.0" encoding="UTF-8"?>
<algorithms_config>
	<alg
		id="1"                                     ... id of algorithm
		name="watch_and_notify"                    ... name of binary builded
		pathOfBinary="Algorithms/watch_and_notify" ... path to binary
		numParams="5"                              ... number of parameters defined
		maxDevs="2"                                ... maximum number of devices for algorithm
		type="1"                                   ... type of Algorithm
	/>
	<alg
		id="3"
		name="geofencing"
		pathOfBinary="Algorithms/geofencing"
		numParams="4"
		maxDevs="1"
		type="2"
	/>
</algorithms_config>


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