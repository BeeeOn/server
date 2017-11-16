#ifndef BEEEON_DESERIALIZER_H
#define BEEEON_DESERIALIZER_H

namespace BeeeOn {

template <typename T>
class Deserializer {
public:
	virtual void partial(T &t) const = 0;
	virtual void full(T &t) const = 0;
};

}

#endif
