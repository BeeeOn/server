/**
 * \page restui.api REST UI API
 * \tableofcontents
 *
 * \section restui_api_introduction Introduction
 *
 * The REST-UI API is intended for gateway management. Each registered user
 * can associate or be associated with a number of gateways. The association
 * is always attributed by a permission level (admin, user, guest).
 * Based on the permissions, a user can do basically the following actions:
 *
 * - register new devices with the gateway,
 * - see measured data from a device,
 * - change status of controllable devices (lights on/off, etc.),
 * - invite other users to use certain gateways.
 *
 * \section restui_api_response Responses
 *
 * Each response contains a JSON object payload having common entries:
 *
 * - "code" - holds the HTTP response code, usually 200
 * - "status" - "success", "error" or "fail"
 * - "message" - short localized message describing the error or fail
 * - "data" - object or array with result of the request
 *
 * An example successful response would look like:
 * <pre>
 * {
 *   "code": 200,
 *   "status": "success",
 *   "data": {...}
 * }
 * </pre>
 *
 * An example error response would look like:
 * <pre>
 * {
 *   "code": 404,
 *   "status": "error",
 *   "message": "no such entity found"
 * }
 * </pre>
 */
