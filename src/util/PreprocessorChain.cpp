#include "di/Injectable.h"
#include "util/PreprocessorChain.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PreprocessorChain)
BEEEON_OBJECT_CASTABLE(Preprocessor)
BEEEON_OBJECT_PROPERTY("chain", &PreprocessorChain::add)
BEEEON_OBJECT_END(BeeeOn, PreprocessorChain)

using namespace std;
using namespace BeeeOn;

void PreprocessorChain::add(Preprocessor::Ptr pp)
{
	m_chain.emplace_back(pp);
}

string PreprocessorChain::process(const string &input) const
{
	string tmp = input;

	for (auto pp : m_chain)
		tmp = pp->process(tmp);

	return tmp;
}
