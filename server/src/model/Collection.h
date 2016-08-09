#ifndef BEEEON_COLLECTION_H
#define BEEEON_COLLECTION_H

#include <vector>

namespace BeeeOn {

/**
 * Generic collection of objects with serialization support.
 */
template <typename T>
class Collection {
public:
	void add(const T &t)
	{
		m_vector.push_back(t);
	}

	T &get(unsigned int i)
	{
		return m_vector[i];
	}

	const T &get(unsigned int i) const
	{
		return m_vector[i];
	}

	template <typename ArraySerializer, typename Serializer>
	void toWeb(ArraySerializer &s)
	{
		typename std::vector<T>::const_iterator it;

		for (it = m_vector.begin(); it != m_vector.end(); ++it) {
			Serializer objectSerializer = s.startObject();
			T &t = *it;

			t.toWeb(objectSerializer);
			s.endObject(objectSerializer);
		}
	}

	template <typename ArraySerializer, typename Serializer>
	void fromWeb(ArraySerializer &s)
	{
		m_vector.reserve(s.size());

		for (unsigned int i = 0; i < s.size(); ++i) {
			Serializer objectSerializer = s.getObject(i);

			m_vector[i].push_back(T());
			m_vector[i].back().fromWeb(objectSerializer);
		}
	}

private:
	typename std::vector<T> m_vector;
};

}

#endif
