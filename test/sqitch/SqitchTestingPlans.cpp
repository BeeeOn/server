#include "sqitch/SqitchTestingPlans.h"

using namespace std;
using namespace BeeeOn;

const string SqitchTestingPlans::WIDGETS(
	"%project=widgets\n"
	"# This is a note\n"
	"\n"
	" # And there was a blank line.\n"
	"\n"
	"hey 2012-07-16T14:01:20Z Barack Obama <potus@whitehouse.gov>\n"
	"you 2012-07-16T14:01:35Z Barack Obama <potus@whitehouse.gov>\n"
	"@foo 2012-07-16T14:02:05Z Barack Obama <potus@whitehouse.gov> # look, a tag!\n"
);

const string SqitchTestingPlans::MULTI(
	"%project=multi\n"
	"# This is a note\n"
	"\n"
	"# And there was a blank line.\n"
	"\n"
	"hey 2012-07-16T17:25:07Z theory <t@heo.ry>\n"
	"you 2012-07-16T17:25:07Z anna <a@n.na>\n"
	"@foo 2012-07-16T17:24:07Z julie <j@ul.ie> # look, a tag!\n"
	"\n"
	"this/rocks  2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"hey-there 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov> # trailing note!\n"
	"@bar 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@baz 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
);

const string SqitchTestingPlans::CHANGES_ONLY(
	"%project=changes_only\n"
	"# This is a note\n"
	"\n"
	"# And there was a blank line.\n"
	"\n"
	"hey 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"you 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"whatwhatwhat 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
);

const string SqitchTestingPlans::BAD_CHANGE(
	"%project=bad_change\n"
	"# This is a note\n"
	"\n"
	"# And there was a blank line.\n"
	"\n"
	"what what what 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov> # OHNOEZ, No white space allowed!\n"
	"\n"
	"\n"
	"\n"
);

const string SqitchTestingPlans::DUPE_TAG(
	"%project=dupe_tag\n"
	"\n"
	"whatever 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@foo 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"\n"
	"hi 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@bar 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"\n"
	"@stink  2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"\n"
	"@blah 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@bar 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@w00t 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"OHNOEZ 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
);

const string SqitchTestingPlans::DEPLOY_AND_REVERT(
	"%project=deploy_and_revert\n"
	"\n"
	"+hey 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"+you 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	" + dr_evil 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@foo 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"\n"
	"+this/rocks  2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	" hey-there 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"-dr_evil 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@bar 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
);

const string SqitchTestingPlans::DEPENDENCIES(
	"%project=dependencies\n"
	"\n"
	"+roles 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"+users     [roles] 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"+add_user  [users roles] 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"+dr_evil 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"@alpha 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"\n"
	"+users [users@alpha] 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"-dr_evil 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
	"+del_user [!dr_evil users] 2012-07-16T17:25:07Z Barack Obama <potus@whitehouse.gov>\n"
);
