#pragma once

#include <Poco/Timestamp.h>
#include <Poco/Nullable.h>

#include "model/AbstractModule.h"
#include "model/Gateway.h"
#include "model/User.h"
#include "model/ValueAt.h"

namespace BeeeOn {

/**
 * @brief Controllable module representation.
 *
 * Control represents a module that can be controlled by user. The control
 * can be performed either by the software or by a manual intervention.
 *
 * Each Control knows its last confirmed/reported value. The value could be
 * reported as either:
 *
 * * successful set of value by software
 * * control reports its value (measuring some value, physical manipulation
 *   by human)
 *
 * To set a new value of a control, the user requests to change the physical
 * module remotely. We record the timestamp at the time of the request. Than,
 * the gateway responds when the request is accepted. After that, the logic
 * reponsible for the module control responses with either successful change
 * failure during the setting. The accepted step can be skipped. In that case,
 * it is assumed that the accepted step is considered to happen at the same
 * time when the change has applied to the physical device.
 *
 * If the gateway nor device responds on time, the system can timeout and set
 * the failure state and timestamp as well.
 */
class Control : public AbstractModule {
public:
	Control();
	Control(const ID &id);

	/**
	 * @brief Representation of a value change request.
	 * The state of the change is encode via timestamps:
	 *
	 * - requestedAt - when the request has started
	 * - acceptedAt - when the request has been accepted by gateway
	 * - finishedAt - when the request has finished
	 *
	 * A request can finish successfully or it might fail.
	 */
	class RequestedValue {
	public:
		enum Result {
			RESULT_UNKNOWN = 0,
			RESULT_FAILURE = 1,
			RESULT_SUCCESS = 2,
		};

		RequestedValue();
		RequestedValue(
			const User &originator,
			const Poco::Timestamp &at,
			double value);

		void setOriginator(const User &originator);

		/**
		 * User that originated the change (request).
		 */
		const User &originator() const;

		void setValue(double value);
		double value() const;

		bool isValueValid() const;

		/**
		 * @returns true if the request has started
		 * (but might be finished already).
		 */
		bool hasStarted() const;

		/**
		 * @returns true if the request is finished and failed.
		 */
		bool hasFailed() const;

		/**
		 * @returns true if the request has started but has not
		 * finished yet - i.e. it is active.
		 */
		bool isActive() const;

		bool failed() const
		{
			return result() == RESULT_FAILURE;
		}

		void setResult(const Result &result);
		Result result() const;

		void setRequestedAt(const Poco::Nullable<Poco::Timestamp> &at);

		/**
		 * The property requestedAt() can be null only in case we want
		 * to represent a control with no request history. Otherwise,
		 * it is always non-null.
		 */
		Poco::Nullable<Poco::Timestamp> requestedAt() const;

		void setAcceptedAt(const Poco::Nullable<Poco::Timestamp> &at);
		Poco::Nullable<Poco::Timestamp> acceptedAt() const;

		void setFinishedAt(const Poco::Nullable<Poco::Timestamp> &at);

		/**
		 * Property finishedAt() determines whether the request has
		 * finished or not and thus whether the operation result is
		 * valid.
		 */
		Poco::Nullable<Poco::Timestamp> finishedAt() const;

	private:
		User m_originator;
		double m_value;
		Result m_result;
		Poco::Nullable<Poco::Timestamp> m_requestedAt;
		Poco::Nullable<Poco::Timestamp> m_acceptedAt;
		Poco::Nullable<Poco::Timestamp> m_finishedAt;
	};

	void setRecentValue(const ValueAt &value);

	/**
	 * @return value reported by the control
	 */
	const ValueAt &recentValue() const;

	void setRequestedValue(const RequestedValue &value);

	/**
	 * @return value reported by the control
	 */
	const RequestedValue &requestedValue() const;


private:
	ValueAt m_recentValue;
	RequestedValue m_requestedValue;
};

typedef Control::ID ControlID;

}
