#ifndef BEEEON_WORK_FACTORY_H
#define BEEEON_WORK_FACTORY_H

#include <string>
#include <typeinfo>

namespace BeeeOn {

class Work;

/**
 * WorkFactory interface. A common ancestor of all Work factories.
 */
class WorkFactory {
public:
	virtual ~WorkFactory();

	virtual std::string id() const = 0;
	virtual Work *create() = 0;

	/**
	 * Provide an identification of the underlying Work class.
	 */
	static std::string identify(const std::type_info &info);
};

/**
 * Generic factory for any Work that has an empty constructor.
 */
template <typename W>
class GenericWorkFactory : public WorkFactory {
public:
	GenericWorkFactory();

	std::string id() const override
	{
		return WorkFactory::identify(typeid(W));
	}

	Work *create() const override;
};

template <typename W>
GenericWorkFactory<W>::GenericWorkFactory():
	WorkFactory(typeid(W))
{
}

template <typename W>
Work *GenericWorkFactory<W>::create() const
{
	return new W;
}

}

#endif
