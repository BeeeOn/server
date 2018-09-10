#pragma once

#include "l10n/Translator.h"
#include "provider/SubtypeInfoProvider.h"
#include "provider/TypeInfoProvider.h"
#include "rest/JSONRestHandler.h"
#include "util/UnsafePtr.h"

namespace BeeeOn {
namespace RestUI {

class TypeRestHandler : public JSONRestHandler {
public:
	TypeRestHandler();

	void setTypeInfoProvider(TypeInfoProvider *provider);
	void setSubtypeInfoProvider(SubtypeInfoProvider *provider);
	void setTranslatorFactory(TranslatorFactory::Ptr factory);

	/**
	 * @brief List all data types that can be used by sensoric devices.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "temperature",
	 *     "name": "temperature",
	 *     "unit": "C",
	 *   },
	 *   {
	 *     "id": "battery",
	 *     "name": "battery",
	 *     "unit": "%"
	 *     "levels": [
	 *       {
	 *         "name": "critical",
	 *         "min": "0",
	 *         "max": "10",
	 *         "attention": "single",
	 *       }
	 *     ]
	 *   }
	 * ]
	 * </pre>
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show detail of a sensoric type identified by its unique ID.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "fire",
	 *   "name": "fire",
	 *   "values": {
	 *     "0": "no fire",
	 *     "1": "fire detected"
	 *   }
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Show detail a subtype that specifies more details of its type.
	 *
	 * A subtype extends definitions of types <code>enum</code> and <code>bitmap</code>.
	 * In this way, it is possible to express and understand vendor-specific data of
	 * certain sensors.
	 *
	 * Subtypes of enum define fields <em>id</em>, <em>name</em> and <em>values</em>.
	 * The <em>values</em> entry lists the particular enum values and their mappings
	 * to the values as could be get by requesting a sensor history.
	 *
	 * Subtypes of bitmap define fields <em>id</em>, <em>name</em>, <em>flags</em>
	 * and <em>groups</em>. Only one or both of <em>flags</em> and <em>groups</em>
	 * can be present. The <em>flags</em> structure specifies mapping of certain
	 * bits to their semantics. The <em>groups</em> array specifies mapping of
	 * certain bits into groups that make sense only all together (e.g. scattered
	 * error bits).
	 *
	 * - usual responses: 200, 404
	 * - example enum output (data):
	 * <pre>
	 * {
	 *   "id": "MOD_BOILER_STATUS",
	 *   "name": "boiler status",
	 *   "values": {
	 *     "0": "undefined",
	 *     "1": "heating",
	 *     "2": "hot water",
	 *     "3": "failure",
	 *     "4": "shutdown",
	 *   }
	 * }
	 * </pre>
	 * - example bitmap flags output:
	 * <pre>
	 * {
	 *   "id": "MOD_CURRENT_BOILER_OT_FAULT_FLAGS",
	 *   "name": "OpenTherm fault flags",
	 *   "flags": {
	 *     "0": {
	 *       "name": "service request",
	 *       "active": "high",
	 *     },
	 *     "1": {
	 *       "name": "lockout reset",
	 *       "active": "high",
	 *     },
	 *     ...
	 *   }
	 * }
	 * </pre>
	 * - example bitmap groups output (bits 0, 1, 2, 4, 6, 7 are to be extracted to
	 *   form a single number - e.g. sensor value 0xca would be transformed into 0x32
	 *   to contain only the significant bits):
	 * <pre>
	 * {
	 *   "id": "MOD_CURRENT_BOILER_OT_OEM_FAULTS",
	 *   "name": "OpenTherm faults",
	 *   "groups": [
	 *     {
	 *       "mapping": [0, 1, 2, 4, 6, 7],
	 *       "name": "OEM specific",
	 *     }
	 *   ]
	 * }
	 * </pre>
	 */
	void detailSubtype(RestFlow &flow);

private:
	UnsafePtr<TypeInfoProvider> m_provider;
	UnsafePtr<SubtypeInfoProvider> m_subtypeProvider;
	TranslatorFactory::Ptr m_translatorFactory;
};

}
}
