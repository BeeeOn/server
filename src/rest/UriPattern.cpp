#include <Poco/Exception.h>
#include <Poco/String.h>
#include <Poco/Logger.h>

#include "rest/UriPattern.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

UriPattern::UriPattern(
		const std::string &pattern,
		const vector<string> &params):
	m_pattern(pattern)
{
	URI uri(m_pattern);
	uri.getPathSegments(m_segments);
	unsigned int i = 0;

	for (auto &segment : m_segments) {
		if (segment.find(PARAM_PREFIX) != 0)
			continue;

		if (i >= params.size()) {
			throw InvalidArgumentException(
				"too many params in URI " + uri.toString());
		}

		if (segment.substr(1) != params[i++]) {
			throw InvalidArgumentException(
				"unexpected param " + segment + " in URI " + uri.toString());
		}

		if (logger().debug()) {
			logger().debug("detected param " + segment.substr(1),
					__FILE__, __LINE__);
		}
	}
}

bool UriPattern::match(Poco::URI uri, Params &params) const
{
	if (m_pattern == "*")
		return true;

	if (m_pattern.empty() || uri.empty())
		return false;

	// relative pattern cannot match absolute path
	if (m_pattern[0] != '/' && uri.getPath()[0] == '/')
		return false;

	vector<string> segments;
	uri.getPathSegments(segments);

	if (segments.size() != m_segments.size())
		return false;

	int i = 0;

	for (auto &expect : m_segments) {
		if (expect.find(PARAM_PREFIX) == 0) {
			params[expect.substr(1)] = segments[i++];
			continue;
		}
		else if (expect == "*") {
			i += 1;
			continue;
		}
		else if (expect == segments[i++]) {
			continue;
		}

		if (logger().debug()) {
			logger().debug("matching failed at " + expect + " ~ " + segments[i - 1],
					__FILE__, __LINE__);
		}

		return false;
	}

	return true;
}

URI UriPattern::format(const vector<string> &params) const
{
	string path;
	size_t i = 0;

	for (auto &segment : m_segments) {
		if (segment.find(PARAM_PREFIX) == 0) {

			if (i >= params.size()) {
				throw InvalidArgumentException(
					"missing parameters to format path "
					+ m_pattern);
			}

			path.append("/");
			path.append(params[i++]);
		}
		else {
			path.append("/");
			path.append(segment);
		}
	}

	if (i < params.size()) {
		throw InvalidArgumentException(
			"too many parameters to format path "
			+ m_pattern);
	}

	return URI(path);
}
