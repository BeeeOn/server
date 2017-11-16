#ifndef BEEEON_WORK_BACKUP_H
#define BEEEON_WORK_BACKUP_H

#include <vector>

#include "work/Work.h"

namespace BeeeOn {

class WorkBackup {
public:
	virtual ~WorkBackup();

	/**
	 * Store work persistently. It solves transactions automatically
	 * unless transaction is true.
	 */
	virtual void store(Work::Ptr work, bool transaction = false) = 0;

	/**
	 * Load all work that is to be executed.
	 */
	virtual void loadScheduled(std::vector<Work::Ptr> &all) = 0;
};

class EmptyWorkBackup : public WorkBackup {
public:
	void store(Work::Ptr work, bool transaction = false) override;
	void loadScheduled(std::vector<Work::Ptr> &all) override;

	static WorkBackup &instance();
};

}

#endif
