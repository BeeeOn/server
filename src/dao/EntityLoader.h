#ifndef BEEEON_ENTITY_LOADER_H
#define BEEEON_ENTITY_LOADER_H

#include <string>

namespace BeeeOn {

/**
 * An EntityLoader knows how to load contents of a certain
 * Entity instance. It has right to mark the instance as
 * loaded.
 */
class EntityLoader {
protected:
	template <typename Entity>
	static void markLoaded(Entity &entity);
};

template <typename Entity>
void EntityLoader::markLoaded(Entity &entity)
{
	entity.setLoaded(true);
}

}

#endif
