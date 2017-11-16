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
* Work Scheduler - allows to perform asynchronous tasks.
* Notification Infrastructure - delivers user notifications to user devices
(phones, tablets, etc.).

## Start hacking

The BeeeOn Gateway is built by the CMake build system:

```
$ mkdir build
$ (cd build && cmake ..)
$ make -C build
```

During development and for testing, the BeeeOn Server can be started as:

```
$ tools/server-testing-start.sh
```

This starts a private instance of the PostgreSQL server and then it starts the server
that sets the database up.

There are multiple startup configurations according to the purpose of testing or debugging.
The server is started either with the REST-UI or with the XML-UI:

```
$ TARGET=xmlui tools/server-testing-start.sh
$ TARGET=restui tools/server-testing-start.sh
```

The executable `beeeon-server` reads a startup configuration file and configuration
files from conf/config.d. Then, the definition of the _main_ instance (of class
LoopRunner) is searched, created and executed. All main application threads start
via this main instance.

## Integration tests

The directory `t/` contains a set of integration tests. Tests are divided by the public server APIs. Thus,
the XML-UI, REST-UI and GWS are tested independently on each other. All tests are written in Python.
See `t/README` for details.
