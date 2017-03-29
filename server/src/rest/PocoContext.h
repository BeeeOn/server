#ifndef BEEEON_POCO_CONTEXT_H
#define BEEEON_POCO_CONTEXT_H

#include <string>

#include <Poco/URI.h>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace BeeeOn {

template <typename Backend>
class PocoContext {
public:
	PocoContext(Backend &backend):
		m_impl(backend)
	{
	}

	bool has(const std::string &key) const
	{
		return m_impl.has(key);
	}

	const std::string at(const std::string &key) const
	{
		return m_impl.get(key);
	}

	const std::string at(const std::string &key, const std::string &def) const
	{
		return m_impl.get(key, def);
	}

	const std::string operator[] (const std::string &key) const
	{
		return at(key);
	}

	typename Backend::ConstIterator begin() const
	{
		return m_impl.begin();
	}

	typename Backend::ConstIterator end() const
	{
		return m_impl.end();
	}

	const Backend &impl() const
	{
		return m_impl;
	}

	Backend &impl()
	{
		return m_impl;
	}

protected:
	Backend &m_impl;
};

class PocoRequest : public PocoContext<Poco::Net::HTTPServerRequest> {
public:
	PocoRequest(Poco::Net::HTTPServerRequest &impl);

	std::istream &stream() const;
	bool accepts(const std::string &type, const std::string &subtype) const;
};

template <typename T>
class PocoHeaderAccessor {
public:
	PocoHeaderAccessor(T &t, const std::string &key):
		m_t(t),
		m_key(key)
	{
	}

	operator std::string()
	{
		return m_t.get(m_key);
	}

	std::string operator =(const std::string &s)
	{
		m_t.set(m_key, s);
		return m_t.get(m_key);
	}

	std::string operator =(const Poco::URI &uri)
	{
		m_t.set(m_key, uri.toString());
		return m_t.get(m_key);
	}

private:
	T &m_t;
	const std::string &m_key;
};

class PocoResponse : public PocoContext<Poco::Net::HTTPServerResponse> {
public:
	PocoResponse(Poco::Net::HTTPServerResponse &impl);

	PocoHeaderAccessor<Poco::Net::HTTPServerResponse> at(const std::string &key);
	PocoHeaderAccessor<Poco::Net::HTTPServerResponse>
		operator[] (const std::string &key)
	{
		return at(key);
	}

	void setStatus(unsigned int status);
	unsigned int status() const;
	void sendAll(const void *p, std::size_t len);
	std::ostream &stream();
};

}

#endif
