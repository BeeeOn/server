# User access policy

Each user frontend maintains a user session in some way. The session identifies
a user which grants her a set of permission rules. The permissions are based
on the selected _access policy_. Usually, for each significant entity, relation
or function in the system, there is a separate policy interface:

* `GatewayAccessPolicy`
* `DeviceAccessPolicy`
* `RoleAccessPolicy`
* `SensorAccessPolicy`
* `SensorHistoryAccessPolicy`
* `ControlAccessPolicy`
* `IdentityAccessPolicy`
* `LocationAccessPolicy`
* `FCMTokenAccessPolicy`
* etc.

There is currently only one implementation of them: `DefaultAccessPolicy`. The
class `DefaultAccessPolicy` contains a hard-wired strategy to grant permissions
to perform certain tasks as defined by the implemented interfaces.

## Access level

BeeeOn Server defines a term _access level_. An access level is basically an
unsigned number where _0_ denotes the **highest permission level** (_root_).
In practice, the system works with more coarse grain access levels. There are
5 levels:

* _ANY_ (0) - useful to define rules (not user levels)
* _ADMIN_ (10) - the highest permission level available to be widely used
* _USER_ (20) - user that can do regular operations with devices
* _GUEST_ (30) - guest with read-only access
* _NONE_ (100) - no access, the lowest defined level to be practically used

## Gateway registration

Each BeeeOn Gateway is registered by its unique ID. The gateway, when connected
for the first time, registers itself with the BeeeOn Server. The user who knows
that ID can register with the gateway and become its maintainer. Such user
is automatically granted the access level _ADMIN_. Nobody else can now register
the gateway.

## Overview of default access policy

The following table summarizes how the `DefaultAccessPolicy` class handles
access policy to each entity.

| Entity          | Action                | _ADMIN_  | _USER_  | _GUEST_ |
|:---------------:|:---------------------:|:--------:|:-------:|:-------:|
| `Control`       | `GET`                 |        y |       y |       y |
| `Control`       | `SET`                 |        y |       y |       n |
| `Device`        | `GET`                 |        y |       y |       y |
| `Device`        | `UNREGISTER`          |        y |       n |       n |
| `Device`        | `ACTIVATE`            |        y |       n |       n |
| `Device`        | `UPDATE`              |        y |       y |       n |
| `Device`        | `UPDATE_AND_ACTIVATE` |        y |       n |       n |
| `FCMToken`      | `REGISTER`            |        y |       y |       y |
| `FCMToken`      | `UNREGISTER`          |        y |       y |       y |
| `Gateway`       | `REGISTER`            | y<sup>1</sup> | y<sup>1</sup> | y<sup>1</sup> |
| `Gateway`       | `UNREGISTER`          | y<sup>4</sup> | y<sup>4</sup> | y<sup>4</sup> |
| `Gateway`       | `GET`                 |        y |       y |       y |
| `Gateway`       | `UPDATE`              |        y |       y |       n |
| `Gateway`       | `SCAN`                |        y |       y |       n |
| `GatewayMessage`| `GET`                 |        y |       y |       n |
| `GatewayMessage`| `REMOVE`              |        y |       n |       n |
| `Identity`      | `GET`                 | y<sup>2</sup> | y<sup>2</sup> | y<sup>2</sup> |
| `Location`      | `GET`                 |        y |       y |       y |
| `Location`      | `CREATE`              |        y |       y |       n |
| `Location`      | `UPDATE`              |        y |       y |       n |
| `Location`      | `REMOVE`              |        y |       y |       n |
| `RoleInGateway` | `INVITE`              |        y |       n |       n |
| `RoleInGateway` | `GET`                 |        y |       y |       y |
| `RoleInGateway` | `UPDATE`              | y<sup>3</sup> |  n |       n |
| `RoleInGateway` | `REMOVE`              | y<sup>4</sup> |  n |       n |
| `Sensor`        | `GET`                 |        y |       y |       y |
| `SensorHistory` | `GET`                 |        y |       y |       y |

_Notes:_

<sup>1</sup> Anybody can register a new gateway that has no _ADMIN_ at that time.
If a gateway has already a user (i.e. at least one _ADMIN_), such gateway cannot
be registered this way. The user needs to be invited to use that gateway. This is
not defined by the access policy. This is defined by the application logic.

<sup>2</sup> Getting information about an identity depends on the dynamic state of the
system and not on the access level. It is possible to see all identities involved in
the gateway where the user involved as well.

<sup>3</sup> _ADMIN_ cannot change her own access level (e.g. make herself a _USER_).

<sup>4</sup> No user can remove her own role via `RoleInGateway::REMOVE`. However, this
is not defined by any access policy. This is defined by the application logic. If there are
no more _ADMIN_ roles associated with a gateway, it is possible to unregister that gateway from
all users at once by removing the gateway (`Gateway::UNREGISTER`). If there are some other
admins, any user can remove the gateway (`Gateway::UNREGISTER`) which in turn removes
her role.

## Gateway owner

Originally, the BeeeOn Server was using the term _gateway owner_. The current
version of the BeeeOn Server does not have any kind of such role. For backwards
compatibility, the owner of the gateway is the user with _ADMIN_ access level
who has first registered with the gateway. Usually, the user who registers a
gateway for the first time is the owner. If there are more _ADMIN_ users invited
and the original owner is removed, the second oldest user with _ADMIN_ access
level is treated as the owner.
