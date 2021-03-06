# Introduction

BeeeOn Server is a software platform intended for remote control of BeeeOn Gateway
instances. It works as a collector of sensor data and a center for driving the
elements with controllable features (turning light on/of, etc.).

The BeeeOn Server is written in C++ and is divided into several parts,
especially:

* UI - the API for clients. Currently, there are two APIs supported. The older XML-UI
that is maintained for backwards compatibility and the newer REST-UI.
* GWS - the Gateway Server is used to maintain connections to BeeeOn Gateways. It
receives measured sensor data and allows to drive the controllable elements.
* Notification Infrastructure - delivers user notifications to user devices
(phones, tablets, etc.).

## Prerequisites

Required prerequisites for development:

* Poco library 1.7.7+ devel (recommended Poco 1.9)
* icu and libicu-devel
* cppunit-devel
* postgresql-server (9.5+, recommended 10+)
* python3
  * testing.postgresql
  * http.client
  * json
  * uuid
  * unittest
  * unittest-tap-reporting

Optional prerequisites for development:

* jansson-devel
* jwt-devel
* cppzmq-devel
* python3
  * selenium
  * phantomjs
  * python-selenium
  * python-oauth2client
* sqitch
* postgresql

Required prerequisites for deployment:

* Poco library 1.7.7+ (recommended Poco 1.9)
* icu and libicu
* postgresql-server (9.5+, recommended 10+)
* postgresql
* sqitch

Optional prerequisites for deployment:

* jansson
* jwt
* zeromq

## Start hacking

The BeeeOn Gateway is built by the CMake build system:

```
$ mkdir build
$ (cd build && cmake ..)
$ make -C build
```

During development and for testing, the BeeeOn Server can be started as:

```
$ beeeon-server -c conf/testing-startup.ini
```

This starts a private instance of the PostgreSQL server and then it starts the server
that sets the database up.

There are multiple startup configurations according to the purpose of testing or debugging.
Please note, that in this mode the frontend and GWS are disconnected to enable running
of automatic frontend tests (during tests, there is no live gateway and thus the communication
with GWS is emulated). This can be easily changed by giving `-Dgateway-rpc.impl=async` on
command line or by editing `conf/testing-startup.ini`.

The executable `beeeon-server` reads a startup configuration file and configuration
files from `conf/config.d`. Then, the definition of the _all_ instance (of class
LoopRunner) is searched, created and executed. All main application threads start
via this all instance.

## Integration tests

The directory `t/` contains a set of integration tests. Tests are divided by the public server APIs. Thus,
the XML-UI, REST-UI and GWS are tested independently on each other. All tests are written in Python.
See `t/README` for details.
