# Installation

The recommended way to install the BeeeOn Server is to use an RPM package.
The RPM package installs the BeeeOn Server software with a configuration
that is to be amended before running. The systemd service is included.

## Before first run

Before the BeeeOn Server can be started, several configuration entries should
be set and the database must be initialized. The configuration can be usually
found at `/etc/beeeon/server`. The main configuration file is called `server-startup.ini`.
The directory `config.d` contains files that are also applied and can be used to
override the main configuration entries. It is recommanded to customize the
configuration settings by creating new INI files there.

At least the following entries must be configured:

* secure.device_properties.passphrase

For details, see the following sections.

### XML-UI Server

The group `xmlui.ssl` configures using of SSL layer for the XML-UI Server.
The available entries are:

* xmlui.ssl.enable - enable SSL layer for the XML-UI Server
* xmlui.ssl.key - file path to private key of the XML-UI Server
* xmlui.ssl.certificate - file path to public key of the XML-UI Server signed by a CA
* xmlui.ssl.authority - file path to CA certificate
* xmlui.ssl.verifyMode - SSL verify mode (none, relaxed, strict, once)

### REST-UI Server

The group `restui.ssl` configures using of SSL layer for the REST-UI Server.
The available entries are:

* restui.ssl.enable - enable SSL layer for the REST-UI Server
* restui.ssl.key - file path to private key of the REST-UI Server
* restui.ssl.certificate - file path to public key of the REST-UI Server signed by a CA
* restui.ssl.authority - file path to CA certificate
* restui.ssl.verifyMode - SSL verify mode (none, relaxed, strict, once)

### Gateway Server (GWS)

The group `gws.ws.ssl` configures using of SSL layer for the GWS.
The available entries are:

* gws.ws.ssl.enable - enable SSL layer for the GWS websockets
* gws.ws.ssl.key - file path to private key of the GWS
* gws.ws.ssl.certificate - file path to public key of the GWS signed by a CA
* gws.ws.ssl.authority - file path to CA certificate
* gws.ws.ssl.verifyMode - SSL verify mode (none, relaxed, strict, once)

Gateways are verified against server by their SSL/TLS certificate. The verification
is influenced either by the SSL settings listed above and by the configuration
entry

* gws.peer.verifier - gateway verification strategy

There are currently the following verification strategies:

* relaxed - all gateways are accepted (for debugging and testing)
* strict - only gateways connected via SSL/TLS connection with a valid certificate
signed by a trusted CA and with a valid gateway ID is accepted

The recommended configuration that is considered safe is:

* gws.ws.ssl.verifyMode = strict
* gws.peer.verifier = strict

### Device Properties

Certain device properties stored in the main database are encrypted at the
application level. For this purpose, a passphrase must be configured to
work properly. The passphrase should be long enough and generated in a
secure random way (e.g. by pwgen). The passphrase must not change, once
it is used (unless the database contents are re-encrypted).

The available entries are:

* secure.device_properties.algorithm - name of algorithm to use (aes256)
* secure.device_properties.passphrase - secure random passphrase

The passphrase must be set. A possible generation method:

```
$ pwgen -s -N 1 64
```

### Database

The only supported database at the moment is the PostgreSQL via ODBC.
There is no need to configure anything unless some non-standard settings
are used. The available entries are:

* database.host - host (should be usually localhost)
* database.port - port where the PostgreSQL instance is listening
* database.user - user name when connecting to database
* database.name - name of database to use (beeeon)

### Database initialization

The database must be initialized before the first run. For this purpose,
there is an initialization script a set of deploy scripts versioned by
the Sqitch management system. The Sqitch is currently a mandatory tool
for initialization. All the necessary scripts can be (usually) found in
`/usr/share/beeeon/db`.

The initialization is done in few steps:

1. Initialize database and create user role `beeeon_user`:

```
$ cd /usr/share/beeeon/db/scripts
$ sudo -u postgres psql -f beeeon_pg_init.sql
```

2. Deploy the database schema by Sqitch:

```
$ SQITCH=$(which sqitch)
$ cd /usr/share/beeeon/db
$ sudo -u beeeon_admin ${SQITCH} deploy beeeon
```

3. Optionally, it is now possible (the database is empty) to execute unit-tests (requires pgtap):

```
$ cd /usr/share/beeeon/db/test
$ sudo -u beeeon_admin pg_prove t*.sql
```

If all the steps has successfully passed, the database is initialized.

### JWT Session Manager

When the JWT Session Manager is enabled, it must be configured accordingly.
The JWT requires to setup the cryptography. The available entries are:

* session.manager - name of manager to use (system, jwt)
* session.jwt.secret - passphrase for signing JWTs
* session.jwt.algorithm - algorithm for signing of JWTs (HS256, HS512, ...)
* session.jwt.audience - names of other system that should be accepting our JWTs

### Client authentication

The OAuth 2.0 login can be setup up in different configurations. Usually, the
particular providers need to be configured differently for various clients:

* web application
* Android application
* iOS application
* etc.

For this purpose, the authentication settings are divided into sections:

* web-app
* android
* any other...

There are, however, settings which can be shared among all those instances.
This is usually the SSL configuration.

#### Google OAuth 2.0

To enable login via the Google OAuth 2.0, it is necessary to configure the
Google ID and secret and to configure the SSL.

The SSL configuration is shared among all Google Auth providers:

* google.ssl.verifyMode - SSL verify mode
* google.ssl.authority - path to CA to verify connection to Google

For web applications, there is the section web-app in the configuration file:

* web-app.google.enable - enable using of the Google OAuth 2.0
* web-app.google.clientId - client ID (obtain from Google)
* web-app.google.clientSecret - secret passphrase (obtain from Google)

For Android applications, there is the section android in the configuration file:

* android.google.enable - enable using of the Google OAuth 2.0
* android.google.clientId - client ID (obtain from Google)
* android.google.clientSecret - secret passphrase (obtain from Google)

#### Facebook OAuth 2.0

To enable login via the Facebook OAuth 2.0, it is necessary to configure the
Facebook ID and secret and to configure the SSL. Similar to the Google OAuth 2.0,
more different configuration are possible while the SSL configuration is shared:

* facebook.ssl.verifyMode - SSL verify mode
* facebook.ssl.authority - path to CA to verify connection to Facebook

For web applications:

* web-app.facebook.enable - enable using of the Facebook OAuth 2.0
* web-app.facebook.clientId - client ID (obtain from Facebook)
* web-app.facebook.clientSecret - secret passphrase (obtain from Facebook)

## Logging

The BeeeOn Server utilizes the Poco Logging Framework for logging purposes.
The initial logging configuration is divided into 2 files:

* server-startup.ini - general configuration
* config.d/logging.ini - configuration of particular loggers

The configuration is prepared for 3 cases:

* logging to the console (useful also for journald)
* logging via Poco::FileChannel that ensures rotations on its own
(this is not a bullet-proof implementation but it just works)
* logging via standard syslog API

The initial configuration enables logging to console (with colors) and via
Poco::FileChannel into the /var/log/beeeon/server.log with rotations.
To change this, please tweak the configuration options:

* logging.channels.split.channels - list of channels to log into
* logging.root.level - logging level of the root logger
* for others, see [FileChannel](https://pocoproject.org/docs/Poco.FileChannel.html),
[SyslogChannel](https://pocoproject.org/docs/Poco.SyslogChannel.html),
[ConsoleChannel](https://pocoproject.org/docs/Poco.ConsoleChannel.html)
and [PatternFormatter](https://pocoproject.org/docs/Poco.PatternFormatter.html) docs.
