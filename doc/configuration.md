# Configuration

BeeeOn Server is a highly configurable system. It has 2 layers of configuration:

* system configuration (factory XML files)
* user configuration (INI and properties files)
* additional definitions and settings

## System configuration

The system configuration defines the whole system. It is possible to create an
entirely different system consisting of different components without recompilation.
The system configuration is however rarely modified or accessed by system administrators
or during deployment unless there is a very important bug fix or security fix.
The system configuration files consist of:

* `config.d/factory-all.xml` - top-level startup code, the _all_ instance
* `config.d/factory-auth.xml` - user authentication components
* `config.d/factory-baf.xml` - automation and data exporting components
* `config.d/factory-dao.xml` - DAO layer and database components
* `config.d/factory-gws.xml` - Gateway Server components
* `config.d/factory-misc.xml` - locale, time zone, Firebase Cloud Messaging and utility components
* `config.d/factory-restui.xml` - REST-UI components
* `config.d/factory-services.xml` - service layer components
* `config.d/factory-ssl.xml` - SSL/TLS components
* `config.d/factory-testing.xml` - PodstgreSQL setup, only for testing purposes
* `config.d/factory-xmlui.xml` - XML-UI components

## User configuration

The user configuration contains a set of configuration entries that can influence
the system's behaviour. These settings are intended to be changed by the system
administrators or during deployment. The user configuration files consist of:

* `server-startup.ini` - most of configuration entries
* `apikeys.properties` - API keys for direct access without OAuth services
* `config.d/logging.ini` - loggers configuration
* `restui-api.ini` - mapping of REST-UI endpoints to their URI and HTTP methods

### General server configuration (server-startup.ini)

The INI file is divided into logcal sections:

* `application` - startup-related settings:
  * `di.runner` - name of the main instance to start (default "all")
  * `di.ldpath` - path to locations from where it is possible to dynamically load shared libraries
  * `security.mode` - security checks done on startup (default: "fail", others: "loud", "ignore")

* `restui` - REST-UI specific settings:
  * `host` - listen address (default: 0.0.0.0)
  * `port` - listen port (default: 8010)
  * `ssl.enable` - enable TLS/SSL for incoming connections
  * `ssl.certificate` - TLS/SSL certificate to used
  * `ssl.key` - TLS/SSL private key
  * `ssl.authority` - TLS/SSL CA
  * `ssl.verifyMode` - mode of verification (none, relaxed, strict, once)
  * `backlog` - sockets backlog
  * `minThreads` - minimal number of allocated threads in dedicated thread pool
  * `maxThreads` - maximal number of allocated threads in dedicated thread pool
  * `threadIdletime` - maximal idle time before thread destruction
  * `cors.origins` - allowed ORIGIN header contents (glob syntax, whitelist, empty allows any)
  * `cors.methods` - allowed HTTP methods (whitelist)
  * `cors.headers` - allowed HTTP headers (whitelist)
  * `cors.maxAge` - max age of CORS preflight

* `xmlui` - XML-UI specific settings:
  * `host` - listen address (default: 0.0.0.0)
  * `port` - listen port (default: 8011)
  * `ssl.enable` - enable TLS/SSL for incoming connections
  * `ssl.certificate` - TLS/SSL certificate to used
  * `ssl.key` - TLS/SSL private key
  * `ssl.authority` - TLS/SSL CA
  * `ssl.verifyMode` - mode of verification (none, relaxed, strict, once)
  * `backlog` - sockets backlog
  * `minThreads` - minimal number of allocated threads in dedicated thread pool
  * `maxThreads` - maximal number of allocated threads in dedicated thread pool
  * `threadIdletime` - maximal idle time before thread destruction
  * `scanDuration` - duration used for gateway discovery (this cannot be set by XML-UI)

* `gws` - GWS specific settings:
  * `ws.ssl.enable` - enable TLS/SSL for incoming connections
  * `ws.ssl.certificate` - TLS/SSL certificate to used
  * `ws.ssl.key` - TLS/SSL private key
  * `ws.ssl.authority` - TLS/SSL CA
  * `ws.ssl.verifyMode` - mode of verification (none, relaxed, strict, once)
  * `peer.verifier` - mode of verification of gateways (relaxed - insecure, strict)
  * `ws.host` - listen address (default: 0.0.0.0)
  * `ws.port` - listen port (default: 8011)
  * `ws.backlog` - sockets backlog
  * `ws.maxMessageSize` - maximal size of the registration message from a gateway (bytes)
  * `ws.minThreads` - minimal number of allocated threads in dedicated thread pool for registration
  * `ws.maxThreads` - maximal number of allocated threads in dedicated thread pool for registration
  * `ws.threadIdletime` - maximal idle time before thread destruction for registration
  * `comm.maxMessageSize` - maximal size of regular message from a gateway (bytes)
  * `comm.receiveTimeout` - socket receive timeout while blocking on read()
  * `comm.sendTimeout` - socket send timeout while blocking on write()
  * `comm.maxBulkDuration` - maximal duration of processing a single incoming bulk of messages
  * `comm.maxBulkSize` - maximal count of messages in a single incoming bulk
  * `comm.minThreads` - minimal number of allocated threads in dedicated thread pool for regular communication
  * `comm.maxThreads` - maximal number of allocated threads in dedicated thread pool for regular communication
  * `comm.threadIdletime` - maximal idle time before thread destruction for regular communication
  * `queue.responseTimeout` - timeout required to be met for a response from a gateway
    `(this is overriden by certain operations, e.g. discovery can take few minutes)
  * `rate.impl` - rate limiter implementation (slidingWindow, no)
  * `rate.perMinute` - maximal number of incoming messages per minute of slidingWindow rate limiter
  * `rate.perHour` - maximal number of incoming messages per hour of slidingWindow rate limiter

* `spec` - location of certain specification and configuration files:
  * `devices.xml` - path to devices.xml to be used
  * `types.xml` - path to types.xml to be used
  * `enums.xml` - path to enums.xml to be used
  * `bitmaps.xml` - path to bitmaps.xml to be used
  * `restui.ini` - path to restui API configuration file to be used

* `bundle` - l18n configuration (using icu)
  * `name` - name of the l18n bundle
  * `resource.dir` - path to the location of preprocessed l18n resources files

* `secure` - passphrase and other security settings
  * `device_properties.algorithm` - crypto algorithm to be used for encyrption of certain device properties
  * `device_properties.passphrase` - random secret passphrase for encyrption of certain device properties

* `session` - session management configuration
  * `manager` - session manager to use (system, jwt)
  * `expire` - session expiration time
  * `maxPerUser` - max count sessions of a single user
  * `jwt.secret` - shared password used for JWT encryption and signing
  * `jwt.algorithm` - singing algorithm (HS256, HS384, HS512, ...)
  * `jwt.audience` - list of audiences to generate JWT for

* `apikeys` - configuration of API keys module
  * `enable` - enable or disable using of API keys
  * `file` - path to the apikeys configuration file (usually in properties format)

* `google` - Google SSL/TLS configuration
  * `ssl.verifyMode` - mode of Google Servers verification over TLS
  * `ssl.authority` - CA sigining the Google Servers TLS certificates

* `facebook` - Facebook SSL/TLS configuration
  * `ssl.verifyMode` - mode of Facebook Servers verification over TLS
  * `ssl.authority` - CA sigining the Facebook Servers TLS certificates

* `web-app` - REST-UI OAuth configuration
  * `google.enable` - enable Google OAuth
  * `google.clientSecret` - client secret to use Google OAuth (private)
  * `google.clientId` - client ID to use Google OAuth (public)
  * `google.redirectUri` - redirect URI used for Google OAuth
  * `facebook.enable` - enabel Facebook OAuth
  * `facebook.clientSecret` - client secret to use Facebook OAuth (private)
  * `facebook.clientId` - client ID to use Facebook OAuth (public)
  * `facebook.redirectUri` - redirect URI used for Facebook OAuth

* `android` - OAuth configuration for accessing from Android (specific for XML-UI)
  * `google.enable` - enable Google OAuth
  * `google.clientSecret` - client secret to use Google OAuth (private)
  * `google.clientId` - client ID to use Google OAuth (public)
  * `google.redirectUri` - redirect URI used for Google OAuth
  * `facebook.enable` - enabel Facebook OAuth
  * `facebook.clientSecret` - client secret to use Facebook OAuth (private)
  * `facebook.clientId` - client ID to use Facebook OAuth (public)
  * `facebook.redirectUri` - redirect URI used for Facebook OAuth

* `tester` - REST-UI tester OAuth configuration (usually same as `web-app`)
  * `google.enable` - enable Google OAuth
  * `google.clientSecret` - client secret to use Google OAuth (private)
  * `google.clientId` - client ID to use Google OAuth (public)
  * `google.redirectUri` - redirect URI used for Google OAuth

* `fcm` - configuration of Firebase Cloud Messaging service
  * `fcm.enable` - enable using of Firebase Cloud Messaging
  * `fcm.senderId` - ID to use for sending FCM notifications
  * `fcm.serverKey` - secret key for sending FCM notifications

* `database` - primary (SQL) database connection configuration
  * `backend` - Poco Data backend to be used (odbc, mysql, ...)
  * `host` - address of the target database server
  * `port` - port of the target database server
  * `user` - user to login into the target database server
  * `name` - name of database
  * `connector` - connection string (usually collects host, port, user, name and others)
  * `pool` - size of database connection pool (usually should be the same as maximal number of threads)
  * `poolBackup` - number of backup threads (to prevent missing connections for some situations)
  * `init` - path to initialization script (SQL)
  * `queries.impl` - query loader implementation (fs, ini)
  * `queries.fs.rootDir` - path to the directory where the queries are located
  * `queries.fs.preserveLines` - preserve lines in SQL queries for reference (1 or 0)
  * `features` - features of the Poco backend (e.g. autoCommit=false)
  * `timeout` - database server connection timeout

* `cassandra` - Cassandra storage connection configuration
  * `enable` - enable support (default: no)
  * `host` - address of the target Cassandra server
  * `port` - port of the target Cassandra server
  * `cluster.name` - name of the Cassandra Cluster
  * `connector` - connection string (usually collects host, port and others)
  * `pool` - size of database connection pool (usually should be the same as maximal number of threads)
  * `poolBackup` - number of backup threads (to prevent missing connections for some situations)
  * `init` - path to initialization script (CQL)
  * `queries.fs.rootDir` - path to the directory where the CQL queries are located
  * `queries.fs.preserveLines` - preserve lines in CQL queries for reference (1 or 0)
  * `features` - features of the Poco Cassandra backend
  * `timeout` - database server connection timeout

* `gateway-rpc` - frontend to GWS communication configuration
  * `type` - RPC implementation (async or fake)
  * `async.defaultTimeout` - default timeout of asynchronous operations, some
    operations overrides this setting

* `baf` - BeeeOn Automation Framework configuration
  * `publish.data.enable` - enable publishing of sensor data events (yes or no)
  * `publish.device.enable` - enable publishing of device-related events (yes or no)
  * `publish.gateways.enable` - enable publishing of gateway connection events (yes or no)
  * `publish.server.enable` - enable publishing of server status events (yes or no)
  * `publish.identity.enable` - enable publishing of identity-related events (yes or no)
  * `publish.zeromq.enable` - enable publishing events over ZeroMQ (yes or no)
  * `publish.zeromq.type` - type of ZeroMQ socket (PUSH, PAIR, PUB, DEALER)
  * `publish.zeromq.endpoint` - socket endpoint (e.g. ipc://var/run/beeeon.zeromq, tcp://127.0.0.1:5000)
  * `publish.zeromq.socket.linger` - socket linger setting (preventing too long `TIME_WAIT` status)

* `events` - internal events' dispatching configuration
  * `finalize.timeout` - how long to wait to deliver events on shutdown

* `logging` - general logging configuration
  * `channels.split.channels` - logging channels to use for logging
  * `loggers.root.level` - level of the root logger
  * `channels.console.pattern` - formatting pattern of the console logger
  * `channels.system.pattern` - formatting pattern of the system logger
  * `channels.system.path` - file to write logs into
  * `channels.system.rotation` - how often to rotate logs
  * `channels.system.flush` - flushing of individual messages (true or false)
  * `channels.system.purgeAge` - how often to purge logs
  * `channels.system.archive` - naming of log files
  * `channels.syslog.pattern` - formatting pattern used when logging via syslog
  * `channels.syslog.name` - name of the syslog identity
  * `channels.syslog.facility` - syslog facility to use (usually `LOG_DAEMON`)
  * `channels.syslog.options` - syslog options (e.g. `LOG_PERROR + LOG_CONS`)

## Additional definitions and settings

There is also a set of files that are part of the configuration and are unlikely
to be changed for deployment. These are:

* `devices.xml` - definitions of supported IoT devices
* `types.xml` - extension of data type definitions
* `bitmaps.xml` - definitions of bitmap subtypes
* `enums.xml` - definitions of enum subtypes
