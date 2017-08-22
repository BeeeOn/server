#ifndef BEEEON_WORK_SERVICE_IMPL_H
#define BEEEON_WORK_SERVICE_IMPL_H

#include "service/WorkService.h"
#include "work/WorkFacade.h"

namespace BeeeOn {

class WorkServiceImpl : public WorkService {
public:
	void setFacade(WorkFacade::Ptr facade);

	bool fetch(Single<Work> &input) override;
	void cancel(Single<Work> &input) override;
	void remove(Single<Work> &input) override;

private:
	WorkFacade::Ptr m_facade;
};

}

#endif
