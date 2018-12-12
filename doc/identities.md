# Identities and users

BeeeOn Server maintains users to be able to restrict access to the associated
gateways and sensors. Users usually log in through some external identity
provider (OAuth 2). The act of registration is merged with the initial login.
BeeeOn Server trusts the external identity providers and when a user passes
a valid token during the login operation, it fetches the necessary user information
from that provider.

![Overview of AuthService](auth-service.png)

## Model

There are 2 main entities: `Identity` and `User`.

A `User` instance represents a person in the system. It contains information
like _first name_, _last name_, _locale_. This information is usually fetched
from an external identity provider.

An `Identity` represents somebody who can but does not have to be registered
with the BeeeOn system. Each identity is identified by _e-mail_. There must
not be two identities of the same _e-mail_. Thus, the identity represents
the person (or people) who controls the connected mail box. By using identity,
it is possible to invite any user to participate in the BeeeOn system regardless
of whether she has already been registered with the BeeeOn system or not.
The invited identity than might use an identity provider associated with that
e-mail to register and use the associated BeeeOn Gateways.

There can be a single user with multiple identities. Also, there can be a single
identity connected with multiple users. However, those two entities are not in
any direct relationship.

![Overview of identity model](identity.png)

When a user registers (logins for the first time) with the BeeeOn system, the
chosen identity provider is used to fetch information about that user. The
instances of `Identity` and `User` are than connected via `VerifiedIdentity`.
This means that a trusted external identity provider has verified that identity.
A `VerifiedIdentity` instance also contains provider-specific details about the
user.

This model allows to connect accounts verified by different identity providers.
If a single _e-mail_ is used with two trusted identity providers and its owner
logins into the BeeeOn system by both of them, the two would be automatically
interconnected. It is also possible (but unimplemented) to interconnect two
different identities (different _e-mails_).

Once an instance of `Identity` is granted an access to a particular BeeeOn Gateway,
all associated identities got automatically the same priviledges (because they
represent the same user).
