#ifndef BEEEON_SQITCH_INITIALIZER_H
#define BEEEON_SQITCH_INITIALIZER_H

#include <functional>
#include <string>

#include <Poco/Path.h>

#include "sqitch/SqitchChange.h"
#include "sqitch/SqitchPlan.h"
#include "util/Loggable.h"

namespace BeeeOn {

class SqitchInitializer : protected Loggable {
public:
	typedef std::function<unsigned int(const std::string &)> ScriptExecutor;

	SqitchInitializer();

	/**
	 * Load and parse the plan file.
	 */
	void loadPlanFile(const std::string &file);

	/**
	 * Set directory where the scripts (dirs deploy/, verify/, revert/)
	 * are located.
	 */
	void setBaseDir(const std::string &dir);

	/**
	 * Set script extension. The default is 'sql'.
	 */
	void setExtension(const std::string &ext);

	/**
	 * Perform deploy operation as implemented by sqitch.
	 * The method should be treated as unsafe due to the fact
	 * that it does not fully implement all sqitch deploy
	 * functionality. The purpose is just to quickly apply
	 * the deployment scripts for testing purposes.
	 */
	void unsafeDeploy(const ScriptExecutor &execute);

protected:
	Poco::Path resolve(
		const std::string &action,
		const SqitchChange::Ptr change,
		const SqitchPlan &plan) const;

	void applyChange(
		const ScriptExecutor &execute,
		const SqitchChange::Ptr change,
		const SqitchPlan &plan) const;
	void applyRevert(
		const ScriptExecutor &execute,
		const SqitchChange::Ptr change,
		const SqitchPlan &plan) const;

private:
	SqitchPlan m_plan;
	Poco::Path m_baseDir;
	std::string m_extension;
};

}

#endif
