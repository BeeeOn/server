#!/bin/bash

for (( i=0; i<10000; i++ ))
do
	#echo "{\"baf\":\"config\",\"user_id\":6,\"task_id\":1,\"relative_id\":$i,\"parameters\":{\"gateway_id\":\"10\",\"notifications\":\"1\"}}" | nc localhost 11234
	echo "{
		    \"baf\": \"config\",
		    \"user_id\": 6,
		    \"task_id\": 2,
		    \"relative_id\": $i,
		    \"parameters\": {
		        \"device_euid\": \"1\",
		        \"module_id\": \"1\",
		        \"operator\": \"greater\",
		        \"value\": \"25.3\",
		        \"notification_text\": \"Hodnota teploty stoupla nad 25.3 stupne!\"
		    }
		}" | nc localhost 11234
done