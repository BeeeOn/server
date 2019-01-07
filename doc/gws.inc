/**
 * \page gws.protocol Gateway Server Protocol
 * \tableofcontents
 *
 * \section gws_protocol_introduction Introduction
 *
 * Gateway Server communicates with BeeeOn Gateways using a specific JSON-based
 * protocol where each message contains a single JSON object. Every message contains
 * field <code>message_type</code> that specifies semantics of the message.
 * Most communication consists of the _request-response_ model. To overcome issues
 * with unreliable transport protocols, certain responses might be acknowladged
 * by a separate _ack_ message.
 *
 * \section gws_protocol_initial Initial sequence
 *
 * Every time when a BeeeOn Gateway connects with the remote server, a common
 * pattern of messages is exchanged:
 *
 * 1. Gateway sends `gateway_register`.
 * 2. Server replies `gateway_accepted`.
 * 3. Gatewaysends `device_list_request` (usually multiple times for different prefixes).
 * 4. Server replies `device_list_response` (for each request).
 * 5. After that, usually, Gateway periodically sends `sensor_data_export` while
 * expecting the `sensor_data_confirm` as an acknowladge.
 *
 * \section gws_protocol_command Command requests
 *
 * There are certain commands that a user might want to execute by a gateway.
 * E.g. when a new device is to be added into the system, she would ask the
 * appropriate gateway to discover all unpaired/unknown devices around and
 * than select the one (or ones) to pair with the BeeeOn system. This would
 * lead to the following message sequence:
 *
 * 1. Server sends `listen_request`.
 * 2. Gateway replies `generic_response` with result `ACCEPTED` (0).
 * 3. Gateway sends `new_device_request` for each discovered device.
 * 4. Server replies `generic_response` with result `SUCCESS` (1) for each known
 * device and `FAILED` (2) for each unknown devices.
 * 5. When the discovery finishes, Gateway sends `generic_response` with result
 * `SUCCESS` (1) or `FAILED` (2) according to the status of the operation.
 *
 * Note that in this case, the `FAILED` result does not necessarily mean that
 * the _discovery_ operation entirely failed. The operation could failed just
 * partially which is currently not covered by the protocol.
 *
 * The example above illustrates how the request-response system works and that
 * both sides can issue requests.
 *
 * At point 5., the gateway should reply with `response_with_ack` that would
 * ask the server to confirm that it has received the response. In that case,
 * the message of type `generic_ack` would expected by the Gateway.
 *
 * \section gws_protocol_issues Known issues
 *
 * There are few issues included in the protocol that have been introduced either
 * by a bad design or bad implementation and should be resolved in the future:
 *
 * - It is not possible to signalize a partial failure, especially for the device
 * discovery operation where a single device manager can fail while all the others
 * work properly.
 *
 * - The message `response_with_ack` seems to be bad by design. Each response should
 * probably has a flag named e.g. `ack_required`. Thus, any response could be
 * acknowladged if necessary.
 *
 * - Messages `last_value_request` and `last_value_response` are obsolete and should
 * not be used anymore. Thus, it might be possible to remove them entirely in the
 * future.
 *
 * - Message `ping_request` is never used. Instead, the WebSocket PING frames are
 * utilized for the same purpose. The message should be removed in the future.
 *
 * - There is no way how to report description of failures to the server.
 */
