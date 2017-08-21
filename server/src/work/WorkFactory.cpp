#include "util/ClassInfo.h"
#include "work/WorkFactory.h"

using namespace BeeeOn;

WorkFactory::~WorkFactory()
{
}

std::string WorkFactory::identify(const std::type_info &info)
{
	return ClassInfo(info).name();
}
