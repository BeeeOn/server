#ifndef BEEEON_SQITCH_TESTING_PLANS_H
#define BEEEON_SQITCH_TESTING_PLANS_H

#include <string>

namespace BeeeOn {

/**
 * Testing plans come mostly from:
 * https://github.com/theory/sqitch/tree/master/t/plans
 */
struct SqitchTestingPlans {
	static const std::string WIDGETS;
	static const std::string MULTI;
	static const std::string CHANGES_ONLY;
	static const std::string BAD_CHANGE;
	static const std::string DUPE_TAG;
	static const std::string DEPLOY_AND_REVERT;
	static const std::string DEPENDENCIES;
};

}

#endif
