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
	echo " * Each gateway must first register itself with the remote server."
	echo " * The server confirms that the gateway was accepted and can send"
	echo " * more messages. Otherwise, the connection would be closed by the"
	echo " * server."
	echo " *"

	for msg in GWGatewayRegister GWGatewayAccepted; do
		generate_msg ${msg}
	done

	echo " * \\section gws_generic Generic messages"
	echo " *"
	echo " * Many commands does not require any special responses. For that"
	echo " * purpose, it is possible to respond with one of the generic"
	echo " * response messages."
	echo " *"

	for msg in GWResponse GWResponseWithAck GWAck; do
		generate_msg ${msg}
	done

	echo " * \\section gws_gwtoserver Requests from gateway"
	echo " *"
	echo " * Gateway can requests certain services from the server as well as"
	echo " * notifying server about certain events. This sections describes"
	echo " * such commands."
	echo " *"

	for msg in GWNewDeviceRequest GWNewDeviceGroupRequest \
			GWDeviceListRequest GWDeviceListResponse \
			GWLastValueRequest GWLastValueResponse \
			GWNoticeRequest; do
		generate_msg ${msg}
	done

	echo " * \\section gws_dataexport Data export messages"
	echo " *"
	echo " * Paired sensors reports measured data to the server. This usually"
	echo " * happens periodically or based on some value's change. Such data"
	echo " * is exported via <code>sensor_data_export</code> message. Every"
	echo " * message must be confirmed by replying <code>sensor_data_confirm</code>."
	echo " *"

	for msg in GWSensorDataExport GWSensorDataConfirm; do
		generate_msg ${msg}
	done

	echo " * \\section gws_serverpair Pairing requests from server"
	echo " *"
	echo " * User can initiate device discovery process and confirm pairing of"
	echo " * the discovered devices. This is accomplished by messages of type"
	echo " * <code>list_request</code>. During the discovery, the gateway generates"
	echo " * <code>new_device_request</code> messages to notify the server about"
	echo " * new discovered or unpaired devices. User can select and confirm devices"
	echo " * to be paired and used. In that case, the server requests the gateway"
	echo " * to accept a selected device by message of type <code>device_accept_request</code>."
	echo " * To unpair a used device, the <code>unpair_request</code> message is used."
	echo " *"

	for msg in GWListenRequest GWDeviceAcceptRequest GWUnpairRequest GWUnpairResponse; do
		generate_msg ${msg}
	done

	echo " * \\section gws_serverreq Other requests from server"
	echo " *"
	echo " * Certain devices can be controlled remotely, e.g. lights can be turn on"
	echo " * or off. To manipulate devices, the server can generate message of type"
	echo " * <code>set_value_request</code>."
	echo " *"

	for msg in GWSetValueRequest; do
		generate_msg ${msg}
	done

	echo " */"
}

test -z "${1}" && ROOT=`pwd` || ROOT="${1}"
generate_docs "${ROOT}"
