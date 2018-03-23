#! /usr/bin/env python3

import json
import logging
import sys
import zmq

def msg_loop(sock):
	while True:
		try:
			message = sock.recv_json()
		except Exception as e:
			logging.error(e)
			continue

		logging.info(str(message))

def start_sub(addr):
	ctx = zmq.Context()
	sock = ctx.socket(zmq.SUB)

	logging.info("connecting to %s" % addr)
	sock.connect(addr)

	logging.info("subscribe to any topic")
	sock.setsockopt(zmq.SUBSCRIBE, b"")

	msg_loop(sock)

def start_pull(addr):
	ctx = zmq.Context()
	sock = ctx.socket(zmq.PULL)

	logging.info("binding to %s" % addr)
	sock.bind(addr)

	msg_loop(sock)

def start_pair(addr):
	ctx = zmq.Context()
	sock = ctx.socket(zmq.PAIR)

	logging.info("binding to %s" % addr)
	sock.bind(addr)

	msg_loop(sock)

def start_router(addr):
	ctx = zmq.Context()
	sock = ctx.socket(zmq.ROUTER)

	logging.info("connecting to %s" % addr)
	sock.connect(addr)

	while True:
		try:
			header, payload = sock.recv_multipart()
			message = json.loads(payload)
		except Exception as e:
			logging.error(e)
			continue

		logging.info("%s - %s" % (str(header), str(message)))

if __name__ == "__main__":
	logging.basicConfig(level = logging.DEBUG)
	logging.info("libzmq %s" % zmq.zmq_version())

	type = "pull"
	addr = "ipc://publish.zeromq"

	if len(sys.argv) > 1:
		type = sys.argv[1]
	if len(sys.argv) > 2:
		addr = sys.argv[2]

	if type == "sub":
		start_sub(addr)
	elif type == "pull":
		start_pull(addr)
	elif type == "pair":
		start_pair(addr)
	elif type == "router":
		start_router(addr)
	else:
		raise Exception("unrecognized type: " + type)
