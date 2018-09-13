#pragma once

#include <set>
#include <string>

#include "sqitch/SqitchEntry.h"
#include "sqitch/SqitchRef.h"

namespace BeeeOn {

class SqitchChange : public SqitchEntry {
public:
	typedef Poco::SharedPtr<SqitchChange> Ptr;

	enum Operation {
		DEPLOY,
		REVERT
	};

	SqitchChange(unsigned int line);

	void setOperation(Operation op);
	Operation operation() const;

	bool addDependency(const SqitchRef &ref);
	std::set<SqitchRef> dependencies() const;

	bool addConflict(const SqitchRef &ref);
	std::set<SqitchRef> conflicts() const;

	std::string toString() const override;

private:
	Operation m_operation;
	std::set<SqitchRef> m_dependencies;
	std::set<SqitchRef> m_conflicts;
};

}
