/**
 * util.h
 * Copyright (C) 2016 Jan Viktorin
 */

#ifndef ADASERVER_UTIL_H
#define ADASERVER_UTIL_H

/**
 * Set the given socket to be reusable after crash.
 */
int makeSocketReusable(int sock);

#endif
