/**
 * util.cpp
 * Copyright (C) 2016 Jan Viktorin
 */

#include <sys/socket.h>
#include "util.h"

int makeSocketReusable(int sock)
{
	const int reuse = 1;

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
			(const char *) &reuse, sizeof(reuse)) < 0)
		return -1;

#ifdef SO_REUSEPORT // since Linux 3.9
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT,
			(const char *) &reuse, sizeof(reuse)) < 0)
		return -1;
#endif

	return 0;
}
