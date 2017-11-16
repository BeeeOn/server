#ifndef BEEEON_CONTROL_H
#define BEEEON_CONTROL_H

#include <Poco/Timestamp.h>
#include <Poco/Nullable.h>

#include "model/AbstractModule.h"
#include "model/Gateway.h"
#include "model/User.h"

namespace BeeeOn {

/**
 * @brief Controllable module representation.
 *
 * Control represents a module that can be controlled by user. The control
 * can be performed either by the software or by a manual intervention.
 *
 * Each control's value is defined by Control::State. Because changing of
 * the control's state is a kind of a distributed transaction, we have to
 * distinguish among value that is currently set, value that is being set
 * and also other situations. Thus, the control's state contains the stability
 * property. The stability is represented by a finite state machine.
 *
 * The most common stability sequence should be:
 *
 * \verbatim
                       ____________gw__________
                      /                        \
                      |                        |
           user       |    gw            gw    V
   UNKNOWN ---> REQUESTED ---> ACCEPTED ---> CONFIRMED
                     A                          |
                     |                          |
                     \____________gw____________/
   \endverbatim
 *
 * The network delays can introduce some UNCONFIRMED stability. In such case,
 * the inpatient user can try to perform another request (with the same or
 * different value).
 *
 * \verbatim
                       ___________________gw____________________
                      /                                         \
                      |                                         |
           user       |    gw            gw               gw    V
   UNKNOWN ---> REQUESTED ---> ACCEPTED ---> UNCONFIRMED ---> CONFIRMED
                  A  A                          |               |
                  |  |                          |               |
                  |  \____________gw____________/               |
                  \_________________________________gw__________/
   \endverbatim
 *
 * Under some circumstances (e.g. a broken motor), the state transition can
 * get stuck (opening is unfinished, sunblinds did not roll up entirely).
 * For this purpose, the stability can be STUCK (somewhere in the middle).
 * The STUCK state can happen even by manual intervention.
 * After the control is fixed, it should report the return to a CONFIRMED
 * state or it can be reset on the server by putting into the UNKNOWN state.
 *
 * \verbatim
           user            gw            gw         gw
   UNKNOWN ---> REQUESTED ---> ACCEPTED ---> STUCK ---> CONFIRMED
       A                                       |
       |                                       |
       \__________________user_________________/
   \endverbatim
 *
 * The control's state change can fail in two possible ways - it fails as
 * FAILED_ROLLBACK and thus the control rollbacks to the last known value or
 * it fails as FAILED_UNKNOWN into an unknown state.
 * Both states can be changed by requesting a new state change. However, in the
 * FAILED_UNKNOWN state, the automatization might deny to continue for safety
 * reasons.
 * \verbatim
                      ________user______________
                     /                          \
                     |                gw---> FAILED_UNKNOWN
                     |               /
           user      V     gw       /    gw
   UNKNOWN ---> REQUESTED ---> ACCEPTED ---> FAILED_ROLLBACK
                     A                           |
                     |                           |
                     \________user_______________/
   \endverbatim
 */
class Control : public AbstractModule {
public:
	Control();
	Control(const ID &id);

	/**
	 * @brief State representation of a certain Control.
	 *
	 * Each controllable module can be in a certain state. The change of
	 * such state can occur immediatelly, it can be delayed or it can
	 * fail. It can also happen that the state change is not confirmed
	 * but phisically occures.
	 */
	class State {
	public:
		State();

		/**
		 * Denotes what is the originator of this state.
		 */
		enum Originator {
			/**
			 * There is no originator (usually invalid situation).
			 */
			ORIGINATOR_NONE,
			/**
			 * Some user is the originator.
			 */
			ORIGINATOR_USER,
			/**
			 * Some gateway is the originator.
			 */
			ORIGINATOR_GATEWAY,
		};

		/**
		 * The control state is changed by inputs from users or
		 * other source. A controllable module can be driven
		 * by software or manually by a human. The stability
		 * reflects whether the state is confirmed to be applied
		 * to the target control or not.
		 */
		enum Stability {
			/**
			 * If a control is has unknown stability, we have no information
			 * from the target controllable module. It is a kind of an invalid
			 * value.
			 */
			STABILITY_UNKNOWN = 0,
			/**
			 * The state is requested to be changed.
			 */
			STABILITY_REQUESTED = 1,
			/**
			 * The request to modify the state of the controllable
			 * module has been accepted and it is in progress.
			 */
			STABILITY_ACCEPTED = 2,
			/**
			 * The modification has probably occured, however, it has
			 * not been confirmed yet. This may happen after the module
			 * change request has been accepted but a timeout has expired.
			 *
			 * The controllable module can be in one of three states:
			 * * the last confirmed state
			 * * this unconfirmed state
			 * * some error state
			 */
			STABILITY_UNCONFIRMED = 3,
			/**
			 * The state is valid and confirmed from the controllable
			 * module. If there is some other manual intervention,
			 * we do not have this information yet.
			 */
			STABILITY_CONFIRMED = 4,
			/**
			 * The state is valid and confirmed. This stability is used
			 * when the gateway changes the control value and informs
			 * the server about such change. In such case, we need to
			 * distinguish between gateway-confirmed and gateway-overriden
			 * and thus the STABILITY_CONFIRMED cannot be used.
			 */
			STABILITY_OVERRIDEN = 5,
			/**
			 * The controllable module got stuck while changing its state.
			 * E.g. a door is not opened but nor it is closed. It is not
			 * probably possible to move them.
			 */
			STABILITY_STUCK = 6,
			/**
			 * The controllable module has failed to set the new value
			 * and stayed in the last confirmed state.
			 */
			STABILITY_FAILED_ROLLBACK = 7,
			/**
			 * The controllable module has failed to set the new value
			 * and its state is unknown.
			 */
			STABILITY_FAILED_UNKNOWN = 8,
			/**
			 * Dummy state.
			 */
			_STABILITY_LAST,
		};

		void setValue(const double value);
		double value() const;

		void setAt(const Poco::Nullable<Poco::Timestamp> &at);
		Poco::Nullable<Poco::Timestamp> at() const;

		void setStability(Stability stability);
		Stability stability() const;

		/**
		 * Set the originator to ORIGINATOR_NONE.
		 */
		void clearOriginator();

		/**
		 * Set the originator to be the given user.
		 */
		void setOriginator(const User &user);

		/**
		 * Set the originator to be the given gateway.
		 */
		void setOriginator(const Gateway &gateway);

		/**
		 * Determine the originator type of this state.
		 */
		Originator originatorType() const;

		/**
		 * @return user originator
		 * @throw IllegalStateException if the originator is not a user
		 */
		const User &user() const;

		/**
		 * @return gateway originator
		 * @throw IllegalStateException if the originator is not a gateway
		 */
		const Gateway &gateway() const;

	private:
		double m_value;
		Poco::Nullable<Poco::Timestamp> m_at;
		Stability m_stability;
		Poco::Nullable<User> m_user;
		Poco::Nullable<Gateway> m_gateway;
	};

	void setLastConfirmed(const Poco::Nullable<State> &state);

	/**
	 * @return the last confirmed state if any
	 */
	const Poco::Nullable<State> &lastConfirmed() const;

	void setCurrent(const State &state);

	/**
	 * @return the current known state (confirmed or not)
	 */
	const State &current() const;

private:
	Poco::Nullable<State> m_lastConfirmed;
	State m_current;
};

typedef Control::ID ControlID;

}

#endif
