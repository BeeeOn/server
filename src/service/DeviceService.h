#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include <list>
#include <vector>

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "model/DeviceProperty.h"
#include "model/Gateway.h"
#include "service/Single.h"
#include "service/Relation.h"
#include "work/Work.h"

namespace BeeeOn {

/**
 * Service for devices management.
 */
class DeviceService {
public:
	typedef Poco::SharedPtr<DeviceService> Ptr;

	virtual ~DeviceService();

	virtual bool fetch(Relation<Device, Gateway> &input) = 0;
	virtual void fetchMany(Single<std::list<Device>> &input) = 0;
	virtual void fetchMany(Relation<std::list<Device>, Gateway> &input) = 0;
	virtual void fetchActiveBy(Relation<std::vector<Device>, Gateway> &input) = 0;
	virtual void fetchInactiveBy(Relation<std::vector<Device>, Gateway> &input) = 0;

	virtual bool activate(Relation<Device, Gateway> &input) = 0;
	virtual Work unregister(Relation<Device, Gateway> &input) = 0;
	virtual bool update(RelationWithData<Device, Gateway> &input) = 0;
	virtual bool updateAndActivate(RelationWithData<Device, Gateway> &input) = 0;

	virtual bool createProperty(RelationWithData<DeviceProperty, Device> &input) = 0;
	virtual bool updateProperty(RelationWithData<DeviceProperty, Device> &input) = 0;
	virtual bool removeProperty(Relation<const DeviceProperty, Device> &input) = 0;
	virtual bool findProperty(Relation<DeviceProperty, Device> &input) = 0;
	virtual void listProperties(Relation<std::list<DeviceProperty>, Device> &input) = 0;
};

}

#endif
