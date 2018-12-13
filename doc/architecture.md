# Architecture

BeeeOn Server is based on the 3-layer architecture. It is divided as:

* Frontend (API) - REST-UI, XML-UI, GWS
* Services - application logic on model objects
* DAO and misc - access to database and external services

![Top-level overview of BeeeOn Server](top-level.png)

## Gateway Server (GWS)

Gateway Server maintains a list of connected BeeeOn Gateways. It receives
measured sensoric data and manages processing of commands and their results.
Each gateway is verified when connected which is usually based on TLS client
certificate verification. To protect the Gateway Server against DoS-like
behaviours (too many messages from a gateway), each gateway's traffic can be
rate limited.

![Overview of Gateway Server](gws.png)

Gateway Server manages a WebSocket server that listens for incoming upgradable
HTTP connections. When a WebSocket connection is established and the remote
gateway is verified, the connection is registered with the `GatewayCommunicator`
that takes care of the rest of the communication.

`GatewayCommunicator` handles incoming messages from gateways via
`Poco::Net::SocketReactor` and (except of low-level WebSocket frames) passes
them to the configured `GWMessageHandler` instance. Everytime a gateway is
connected, disconnected or re-connected (this means 2 consecutive connections
from the same gateway without disconnection in between), an event is generated
and propagated into the configured `GatewayWatcher` instances.

`GatewayPeerVerifierFactory` can create a kind of `GatewayPeerVerifier` instances
that knows how to verify a gateway. The `X509GatewayPeerVerifier` implementation
(and its factory) verifies TLS client certificate of the given `Poco::Net::StreamSocket`
and checks whether the TLS certificate contains the same _gateway ID_ in its _common
name_ as presented in the received registration message from the gateway.
To relax the rules (especially for testing or debugging purposes), there is a class
named `RelaxedGatewayPeerVerifier` (and factory) that wraps any other implementation
while avoiding failing of the true verification process. Using it is **not intended
for production use**.

When a gateway is too agressive in sending messages (due to a bug or a malicious
behaviour), each gateway connection is associated with an instance of `GatewayRateLimiter`.
Such instance is used to deny reception of messages under certain conditions. There
are at least two implementations (with factories): `NoGatewayRateLimiter` that permits
all messages and `SlidingWindowRateLimiter` that limits the incoming message rate
per minute or/and per hour.

## REST-UI

The REST-UI is a HTTP(s) server that handles REST requests. It is protected
by an authentication layer and configurable through INI configuration file
usually named `restui-api.ini`.

![Overview of REST-UI](rest-ui.png)

### Processing requests

All incoming HTTP requests are processed by `PocoRestServer` that wraps the
`Poco::Net::HTTPServer` instance:

1. The server preprocesses HTTP headers (and body) via configurable component
`HTTPFilterChain`. E.g. CORS is implemented by a separate `HTTPFilter` in this
chain.

2. Because nearly all REST endpoints require authentication, it is necessary to
check the user session as soon as possible to determine whether the request is valid.
For this purpose, the `PocoRestServer` uses an instance of `SessionVerfier`.

3. Valid requests are than routed via a `RestRouter` into the target handling
component. Mapping between URI and handlers is described in the `restui-api.ini` file.
The mappings are sometimes called _endpoints_. Each mapping has associated a set of
properties (described later).

4. `PocoRestServer` manages locale and languages for user. It determines the user's
locale from 2 sources: `Accept-Language` header or locale stored for each user in the
database. The user locale is preferred. It is possible to force using of the
Accept-Language header via a custom header `X-Prefer-Request-Language: yes`.

5. The `PocoRestServer` internally uses 2 special endpoints: `builtin.noroute` and
`builtin.unauthorized` which can be associated to a special handler. This is done
during the `RestRouter` initialization by injecting a particular handler implementing
the appropriately named `builtin` actions.

### Configuration of endpoints

Each endpoint is defined by a mapping between its URI and handler. There can be multiple
URIs for a single handler. Each handler is identified by ID consisting of _group name_
and _call name_. In the file `restui-api.ini`, the mappings are described as:

```
api.call.<GROUP>.<CALL>.uri = <URI>
```

Example of the authentication endpoints:

```
api.call.auth.list.uri = /auth
api.call.auth.list.method = GET
api.call.auth.list.session.required = no
api.call.auth.list.input.maxsize = 0
api.call.auth.list.caching = 360

api.call.auth.list.uri = /auth
api.call.auth.list.method = POST
api.call.auth.list.session.required = no
api.call.auth.list.input.maxsize = 1024
api.call.auth.list.caching = no
```

You can see that `GET /auth` would list authentication methods while `POST /auth`
is used to perform login. Both endpoints has other configuration settings
instructing the PocoRestServer to behave accordingly.

The property `api.call.<FULL_NAME>.session.required` determines whether the endpoint
can be accessed without a session or whether a session is required. Is a session is
required and missing, the request is redirected internally to the special endpoint
`builtin.unauthorized`.

It is possible to limit size of each request's body. This is usually the case of
POST and PUT methods. Too big body is automatically refused with HTTP code 400.
The check for input size can be disabled by setting the
`api.call.<FULL_NAME>.input.maxsize` to a negative value (-1).

Some requests might be cached on client or some caching proxy layer. Such endpoints
can be configured via setting `api.call.<FULL_NAME>.caching` to send advertise HTTP
headers Cache-Control and Expires. The behaviour of this features works as follows:

* for builtin endpoints `Cache-Control: public, no-cache` is always inserted
* when value of `caching` is _off_ or _ignore_, no caching header is inserted for
that endpoint
* when value of `caching` is _never_ or _no_, the `Cache-Control: public, no-cache`
is inserted
* when `caching` is a number (in seconds), 2 HTTP headers are inserted:
`Cache-Control: max-age=<value>, must-revalidate` and the second `Expires: <now + value>`

## Internal interaction with GWS

Communication between the BeeeOn Server core and GWS is necessary to allow users to
configure their gateways. Currently, the core and GWS are separated by the GatewayRPC
interface which performs asynchronous calls.

![Components used for communication with GWS](gws-dependencies.png)

The implementation `AsyncGatewayRPC` directly accesses the `GatewayCommunicator` to issue
command requests to remote gateways. The command results are communicated back via the
`GWMessageHandler`. All outstanding commands are temporarily stored in `GWResponseExpectedQueue`
waiting for state update, finish, fail or timeout.

Design of `GatewayRPC` is a subject of rework because the components are too tightly coupled
together. Releasing the relations would allow to run Gateway Server as a standalone application.
This might be useful e.g. to implement load balancing.

## Authentication

Authentication of users into the application is implemented by an instance of `AuthService`
interface. The default implementation `AuthServiceImpl` uses a list of registered
`AuthProvider` instances. BeeeOn Server is intentionally not designed to implement authorization
of users directly, instead all authorization is usually implemented via OAuth. Users are thus
registered automatically when a configured remote authorization service allows to.

![Overview of AuthService](auth-service.png)

It is possible to list all available login methods (registered `AuthProvider` instances).
The user can select which login method to use and perform the associated steps. There are currently
3 `AuthProvider` implementations:

* `PermitAuthProvider` - intended for **testing purposes** as it can login **ANY** existing user
without verification
* `ApiKeyAuthProvider` - reads a properties file (usually `apikeys.properties`) that contains
mappings between
API keys and users
* `OAuth2AuthProvider` - uses an external OAuth 2 service that authorizes users in their own way

The OAuth2AuthProvider uses an instance of `OAuth2CodeExchanger` and `OAuth2UserFetcher` to
process the authorization token from a user. The `OAuth2CodeExchanger` verifies the token with
the remote service and `OAuth2UserFetcher` fetches information about the user. The implementations
for Google and Facebook OAuth services are provided. Other services might require a custom implementation
of one of those interfaces. Support of certain services can be added easily via `GenericCodeExchanger`
implements a generic and configurable way to exchange the tokens. If the service provides user information
in the OpenID format (JWT with some standardized fields), the user data can be decoded via `OpenIDUserDecoder`.
