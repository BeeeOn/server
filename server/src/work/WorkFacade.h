#ifndef BEEEON_WORK_FACADE_H
#define BEEEON_WORK_FACADE_H

namespace BeeeOn {

class Work;

class WorkFacade {
public:
	virtual void schedule(Work &work) = 0;
	virtual void wakeup(Work &work) = 0;
	virtual void cancel(Work &work) = 0;
	virtual bool fetch(Work &work) = 0;
	virtual bool remove(const Work &work) = 0;
};

class NullWorkFacade : public WorkFacade {
public:
	void schedule(Work &work) override;
	void wakeup(Work &work) override;
	void cancel(Work &work) override;
	bool fetch(Work &work) override;
	bool remove(const Work &work) override;

	static WorkFacade &instance();
};

}

#endif
