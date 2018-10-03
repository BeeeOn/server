#! /bin/sh

generate_msg()
{
	echo " * \\subsection gws_${msg} ${msg}"
	echo " * \\copybrief BeeeOn::${msg}"
	echo " *"
	echo " * \\copydetails BeeeOn::${msg}"
	echo " *"
}

generate_docs()
{
	awk '/^ \*\// {next} {print}' "${1}/doc/gws.inc"
	echo " *"

	echo " * \\section gws_registration Registration with server"
	echo " *"

	for msg in GWGatewayRegister GWGatewayAccepted; do
		generate_msg ${msg}
	done

	echo " * \\section gws_generic Generic messages"
	echo " *"

	for msg in GWResponse GWResponseWithAck GWAck; do
		generate_msg ${msg}
	done

	echo " * \\section gws_gwtoserver Requests from gateway"
	echo " *"

	for msg in GWNewDeviceRequest GWNewDeviceGroupRequest \
			GWDeviceListRequest GWDeviceListResponse \
			GWLastValueRequest GWLastValueResponse \
			GWNoticeRequest; do
		generate_msg ${msg}
	done

	echo " * \\section gws_dataexport Data export messages"
	echo " *"

	for msg in GWSensorDataExport GWSensorDataConfirm; do
		generate_msg ${msg}
	done

	echo " * \\section gws_serverpair Pairing requests from server"
	echo " *"

	for msg in GWListenRequest GWDeviceAcceptRequest GWUnpairRequest GWUnpairResponse; do
		generate_msg ${msg}
	done

	echo " * \\section gws_serverreq Other requests from server"
	echo " *"

	for msg in GWSetValueRequest; do
		generate_msg ${msg}
	done

	echo " */"
}

test -z "${1}" && ROOT=`pwd` || ROOT="${1}"
generate_docs "${ROOT}"
