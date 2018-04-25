#ifndef BEEEON_GW_MESSAGE_HANDLER_IMPL_H
#define BEEEON_GW_MESSAGE_HANDLER_IMPL_H

#include "gwmessage/GWDeviceListRequest.h"
#include "gwmessage/GWLastValueRequest.h"
#include "gwmessage/GWNewDeviceRequest.h"
#include "gwmessage/GWSensorDataExport.h"
#include "gwmessage/GWResponse.h"
#include "gwmessage/GWResponseWithAck.h"
#include "gws/DeviceListener.h"
#include "gws/GatewayCommunicator.h"
#include "gws/GWMessageHandler.h"
#include "gws/GWResponseExpectedQueue.h"
#include "gws/RPCForwarder.h"
#include "gws/SensorDataListener.h"
#include "model/DeviceDescription.h"
#include "service/GWSDeviceServiceImpl.h"
#include "service/GWSSensorHistoryServiceImpl.h"
#include "util/EventSource.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Provides an implementation of the GWMessageHandler. It handles GWRequest,
 * GWResponse or GWSensorDataExport.
 *
 * The purpose of the GWMessageHandlerImpl is to handle requests coming from a
 * BeeeOn Gateway and reply responses or acknowladges. The handling logic is
 * however defered into the service layer. Thus, the main responsibility here
 * is to ensure that responses and acknowladges are properly send and deliver
 * (in best-effort) to the originating gateway.
 */
class GWMessageHandlerImpl :
	public GWMessageHandler,
	protected Loggable {
public:
	void handle(const GWMessage::Ptr message,
			const GatewayID &gatewayID) override;

	void setGatewayCommunicator(GatewayCommunicator::Ptr communicator);
	void setGWResponseExpectedQueue(GWResponseExpectedQueue::Ptr queue);
	void setRPCForwarder(RPCForwarder::Ptr forwarder);
	void setDeviceService(GWSDeviceService::Ptr service);
	void setSensorHistoryService(GWSSensorHistoryService::Ptr service);

	void registerDataListener(SensorDataListener::Ptr listener);
	void registerDeviceListener(DeviceListener::Ptr listener);
	void setEventsExecutor(AsyncExecutor::Ptr executor);

	/**
	 * Break dependency cycles prior destruction.
	 */
	void cleanup();

private:
	/**
	 * @brief On the basis of the request type, the corresponding method
	 * and service in its is called and the response is created and sent.
	 */
	void handleRequest(GWRequest::Ptr request, const GatewayID &gatewayID);

	/**
	 * @brief Response is unregistered from the GWResponseExpectedQueue
	 * and it is forwarded to the UIServer. If a response requires acknowledge,
	 * it is created and sent.
	 */
	void handleResponse(GWResponse::Ptr response, const GatewayID &gatewayID);

	/**
	 * @brief Inserts all sensor data using sensor history service and then
	 * send confirmation message.
	 */
	void handleSensorData(GWSensorDataExport::Ptr dataExport,
			const GatewayID &gatewayID);

	GWResponse::Ptr handleDeviceList(GWDeviceListRequest::Ptr request,
			const GatewayID &gatewayID);
	GWResponse::Ptr handleLastValue(GWLastValueRequest::Ptr request,
			const GatewayID &gatewayID);
	GWResponse::Ptr handleNewDevice(GWNewDeviceRequest::Ptr request,
			const GatewayID &gatewayID);

	static DeviceDescription sanitizeDeviceDescription(
		const DeviceDescription& description);

private:
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWResponseExpectedQueue::Ptr m_responseExpectedQueue;
	RPCForwarder::Ptr m_rpcForwarder;
	GWSDeviceService::Ptr m_deviceService;
	GWSSensorHistoryService::Ptr m_sensorHistoryService;
	EventSource<SensorDataListener> m_dataEventSource;
	EventSource<DeviceListener> m_deviceEventSource;
};

}

#endif
