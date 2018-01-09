#! /usr/bin/gawk -f

function log_trim_left(line)
{
	sub(/^.+\[(Trace|Debug|Information|Notice|Warning|Error|Critical|Fatal)\] /, "", line)
	return line
}

function log_trim_right(line)
{
	sub(/\([^)]*:[0-9]*\)$/, "", line)
	return line
}

function log_extract_pid_tid(line)
{
	sub(/:\{/, "-", line)
	sub(/,.+$/, "", line)

	return line
}

function extract_message(line)
{
	line = log_trim_left(line)
	return log_trim_right(line)
}

function time2ms(time, part, result)
{
	split(time, part, "[:.]")

	result = part[1] * 60 * 60 * 1000
	result += part[2] * 60 * 1000
	result += part[3] * 1000
	result += part[4]

	return result
}

function time_stat(start, end)
{
	if (start == 0)
		return "??:??:??.???.." end " ?ms"

	return start ".." end " " (time2ms(end) - time2ms(start)) "ms"
}

function report_counts(title, counts)
{
	if (counts[0] > 0) {
		print title ":", counts[0]

		for (key in counts) {
			if (key == 0)
				continue

			print "*", key ":", counts[key]
		}
	}
}

function clear_array(array, first_zero) {
	for (key in array)
		delete array[key]

	if (first_zero)
		array[0] = 0
}

function count_array(array, key, count) {
	count = 0
	for (key in array)
		count += 1

	return count
}

function report_server_stats()
{
	report_counts("Criticals", criticals)
	report_counts("Fatals", fatals)

	report_counts("XML-UI clients", xmlui_clients)

	if (xmlui_requests[0] > 0) {
		print "Requests"

		for (key in xmlui_requests) {
			if (key == 0)
				continue

			split(key, multikey, SUBSEP)
			if (multikey[2] != "ip")
				continue

			ip = xmlui_requests[multikey[1], "ip"]
			start = xmlui_requests[multikey[1], "start"]
			end = xmlui_requests[multikey[1], "end"]
			info = xmlui_requests[multikey[1], "info"]

			print "*", ip ":", time_stat(start, end), info
		}
	}

	report_counts("REST-UI clients", restui_clients)

	if (count_array(restui_requests_total) > 0) {
		print "REST-UI Requests"
		for (key in restui_requests_total) {
			print "*", key ":", restui_requests_total[key]
		}
	}

	if (count_array(restui_requests_uri) > 0) {
		print "REST-UI URIs"
		for (key in restui_requests_uri) {
			print "*", key ":", restui_requests_uri[key]
		}
	}

	report_counts("GWS clients", gws_clients)

	if (durations[0] > 0) {
		print "Durations"

		for (key in durations) {
			if (key == 0)
				continue

			split(key, multikey, SUBSEP)
			if (multikey[2] != "duration")
				continue

			clazz = durations[multikey[1], "class"]
			duration = durations[multikey[1], "duration"]
			time = durations[multikey[1], "time"]
			date = durations[multikey[1], "date"]
			thread = durations[multikey[1], "thread"]

			if (date != 0)
				datetime = date " " time
			else
				datetime = time

			duration_per_clazz[clazz] += duration
			count_per_clazz[clazz] += 1

			if (min_per_clazz[clazz] == 0 || duration < min_per_clazz[clazz]) {
				min_per_clazz[clazz] = duration
				min_per_clazz[clazz, "info"] = thread " " datetime
			}

			if (max_per_clazz[clazz] == 0 || duration > max_per_clazz[clazz]) {
				max_per_clazz[clazz] = duration
				max_per_clazz[clazz, "info"] = thread " " datetime
			}
		}

		for (clazz in count_per_clazz) {
			_avg = duration_per_clazz[clazz] / count_per_clazz[clazz]
			_avg = "avg " _avg "us"
			_min = "min " min_per_clazz[clazz] "us"
			_min_info = "(" min_per_clazz[clazz, "info"] ")"
			_max = "max " max_per_clazz[clazz] "us"
			_max_info = "(" max_per_clazz[clazz, "info"] ")"
			_total = "total " count_per_clazz[clazz]

			print "* " clazz ": " _avg " " _total
			print "  " _min " " _min_info
			print "  " _max " " _max_info

			delete count_per_clazz[clazz]
			delete duration_per_clazz[clazz]
			delete min_per_clazz[clazz]
			delete min_per_clazz[clazz, "info"]
			delete max_per_clazz[clazz]
			delete max_per_clazz[clazz, "info"]
		}
	}

	if (transactions[0] > 0) {
		print "Transactions"

		sort = "sort -b -n -k4,4 -r"

		for (key in transactions) {
			if (key == 0)
				continue

			split(key, multikey, SUBSEP)
			if (multikey[2] != "id")
				continue

			id = transactions[multikey[1], "id"]
			type = transactions[multikey[1], "type"]
			start = transactions[multikey[1], "start"]
			end = transactions[multikey[1], "end"]

			print "*", id ":", time_stat(start, end), type | sort
		}

		close(sort)
	}
}

function clear_stats()
{
	clear_array(fatals, 1)
	clear_array(criticals, 1)
	clear_array(xmlui_clients, 1)
	clear_array(xmlui_requests, 1)
	clear_array(restui_clients, 1)
	clear_array(restui_requests_total, 0)
	clear_array(restui_requests_uri, 0)
	clear_array(gws_clients, 1)
	clear_array(transactions, 1)
	clear_array(thread_pools, 1)
	clear_array(durations, 1)

	types_count = 0
	enums_count = 0
	devs_count = 0
	queries_count = 0
	instances_count = 0
	rest_mappings = 0
}

BEGIN {
	clear_stats()
	HAS_DATE = 0
}

{
	if (NR % 13759 == 0) {
		printf "\r%u", NR > "/dev/stderr"
	}
}

/ (19[0-9]{2}|20[0-9]{2})-[012]?[0-9]-[0-3][0-9] / {
	HAS_DATE = 1
}

!/ (19[0-9]{2}|20[0-9]{2})-[012]?[0-9]-[0-3][0-9] / {
	HAS_DATE = 0
}

/PermitAuthProvider/ && /SOME AUTHS/ {
	print
}

/PermitAuthProvider/ {
	next
}

/\[Fatal\]/ {
	_line = $0
	_msg = extract_message($0)

	if (fatals[_msg] == 0)
		fatals[0] += 1

	fatals[_msg] += 1
	if (fatals[_msg] == 1)
		print line
}

/\[Critical\]/ {
	_line = $0
	_msg = extract_message($0)

	if (criticals[_msg] == 0)
		criticals[0] += 1

	criticals[_msg] += 1
	if (criticals[_msg] == 1)
		print line
}

/^Application / && / version / {
	report_server_stats()
	clear_stats()

	if (HAS_DATE)
		_time = $2 " " $3
	else
		_time = $2

	print "# Server run report " _time
	print extract_message($0)
}
/^Application / && / Poco library / {
	print extract_message($0)
}

/IcuLocaleManager/ && /using ICU / {
	print extract_message($0)
}

/IcuLocaleManager/ && /resource dir: / {
	print extract_message($0)
}

/IcuLocaleManager/ && /detected locale: / {
	$0 = log_trim_left($0)
	print "locale: ", $3
}

/PocoDaoManager/ && /initialize database/ {
	print extract_message($0)
}

/PocoDaoManager/ && /Session / && /transact/ {
	print extract_message($0)
}

/PocoSqitchInitializer/ && /unsafe deploy / {
	print extract_message($0)
}

/PocoSimpleDBInitializer/ && /executed script / {
	print extract_message($0)
}

/TypeInfoProvider/ && /register type / {
	types_count += 1
}

/EnumInfoProvider/ && /register enum / {
	enums_count += 1
}

/DevicesSAXHandler/ && /Warning/ {
	print extract_message($0)
}

/DeviceInfoProvider/ && /register device / {
	devs_count += 1
}

/ loading query / {
	queries_count += 1
}

/DependencyInjector/ && /successfully created / {
	instances_count += 1
}

/RestRouter/ && /mapping / {
	rest_mappings += 1
}

/^Application/ && /starting runner/ {
	print extract_message($0)
}

/ creating thread pool / {
	$0 = log_trim_left($0)
	_curr = thread_pools[0] + 1
	thread_pools[0] = _curr
	thread_pools[_curr, "min"] += $5
	thread_pools[_curr, "max"] += $7
}

/LoopRunner/ && / started / && / loops / {
	print "Value types:   ", types_count
	print "Enum subtypes: ", enums_count
	print "Device types:  ", devs_count
	print "DB queries:    ", queries_count
	print "DI instances:  ", instances_count
	print "REST mappings: ", rest_mappings

	$0 = log_trim_left($0)
	print "Loops:         ", $2

	threads_min = 0
	threads_max = 0

	for (i = 0; i < thread_pools[0]; ++i) {
		threads_min += thread_pools[i + 1, "min"]
		threads_max += thread_pools[i + 1, "max"]
	}

	print "Thread pools:  ", thread_pools[0]
	print "Threads:       ", threads_min ".." threads_max
}

/XmlRequestHandlerFactory/ && / accepting client / {
	_id = log_extract_pid_tid($(3 + HAS_DATE))
	$0 = log_trim_left($0)

	split($3, _addr, ":")
	_ip = _addr[1]

	if (xmlui_clients[_ip] == 0)
		xmlui_clients[0] += 1

	xmlui_clients[_ip] += 1
	xmlui_requests_tmp[_id, "ip"] = _ip
}

/XmlRequestHandlerFactory/ && /document: <request/ {
	_id = log_extract_pid_tid($(3 + HAS_DATE))
	xmlui_requests_tmp[_id, "start"] = $(2 + HAS_DATE)

	_msg = extract_message($0)

	if (match(_msg, / ns="[^"]+" /) > 0) {
		_ns = substr(_msg, RSTART, RLENGTH)
		sub(/^.*="/, "", _ns)
		sub(/".*$/, "", _ns)
	}
	if (match(_msg, / type="[^"]+" /) > 0) {
		_type = substr(_msg, RSTART, RLENGTH)
		sub(/^.*="/, "", _type)
		sub(/".*$/, "", _type)
	}

	xmlui_requests_tmp[_id, "info"] = _ns ":" _type
}

/XmlRequestHandler/ && /<response/ {
	_id = log_extract_pid_tid($(3 + HAS_DATE))
	_curr = xmlui_requests[0] + 1
	xmlui_requests[0] = _curr

	xmlui_requests[_curr, "ip"] = xmlui_requests_tmp[_id, "ip"]
	delete xmlui_requests_tmp[_id, "ip"]
	xmlui_requests[_curr, "start"] = xmlui_requests_tmp[_id, "start"]
	delete xmlui_requests_tmp[_id, "start"]
	xmlui_requests[_curr, "info"] = xmlui_requests_tmp[_id, "info"]
	delete xmlui_requests_tmp[_id, "info"]

	xmlui_requests[_curr, "end"] = $(2 + HAS_DATE)
}

/PocoRestRequestFactory/ && /handling request/ {
	$0 = extract_message($0)
	restui_requests_total[$3] += 1
	restui_requests_uri[$4] += 1

	split($6, _addr, ":")
	_ip = _addr[1]

	if (restui_clients[_ip] == 0)
		restui_clients[0] += 1

	restui_clients[_ip] += 1
}

/WebSocketRequestHandler/ && /connection from / {
	$0 = log_trim_left($0)

	split($3, _addr, ":")
	_ip = _addr[1]

	if (gws_clients[_ip] == 0)
		gws_clients[0] += 1

	gws_clients[_ip] += 1
}

/ transaction / && / started / {
	_time = $(2 + HAS_DATE)
	_thread = log_extract_pid_tid($(3 + HAS_DATE))
	$0 = extract_message($0)
	_id = $2 "-" _thread
	transactions_tmp[_id, "start"] = _time
}

/ transaction / && (/ commit / || / rollback /) {
	_time = $(2 + HAS_DATE)
	_thread = log_extract_pid_tid($(3 + HAS_DATE))
	$0 = extract_message($0)
	_id = $2 "-" _thread
	_type = $3

	_curr = transactions[0] + 1
	transactions[0] = _curr

	transactions[_curr, "start"] = transactions_tmp[_id, "start"]
	delete transactions_tmp[_id, "start"]

	transactions[_curr, "end"] = _time
	transactions[_curr, "type"] = _type
	transactions[_curr, "id"] = _id
}

/ duration: / {
	_clazz = $1
	_thread = log_extract_pid_tid($(3 + HAS_DATE))

	if ($5 == "duration:")
		_duration = $6
	else if ($6 == "duration:")
		_duration = $7

	sub(/us$/, "", _duration)

	_curr = durations[0] + 1
	durations[0] = _curr

	durations[_curr, "class"] = _clazz
	durations[_curr, "thread"] = _thread

	if (HAS_DATE) {
		durations[_curr, "date"] = $2
		durations[_curr, "time"] = $3
	}
	else {
		durations[_curr, "time"] = $2
	}

	durations[_curr, "duration"] = int(_duration)
}

END {
	report_server_stats()
	clear_stats()
}
