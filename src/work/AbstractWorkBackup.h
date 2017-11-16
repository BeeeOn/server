#ifndef BEEEON_ABSTRACT_WORK_REPOSITORY_H
#define BEEEON_ABSTRACT_WORK_REPOSITORY_H

#include <map>
#include <typeinfo>

#include "work/WorkBackup.h"

namespace BeeeOn {

class WorkFactory;

class AbstractWorkBackup : public WorkBackup {
public:
	AbstractWorkBackup();

	void store(Work::Ptr work, bool transaction = false) override;

	void registerFactory(WorkFactory *factory);

protected:
	virtual void doStore(
		Work::Ptr work,
		const std::string &id,
		bool transaction) = 0;

	Work *create(const std::string &type, const WorkID &id);

private:
	std::map<std::string, WorkFactory *> m_registry;
};

}

#endif
