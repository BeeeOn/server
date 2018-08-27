#include <Poco/DirectoryIterator.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/Logger.h>
#include <Poco/RegularExpression.h>
#include <Poco/StreamCopier.h>
#include <Poco/StringTokenizer.h>

#include "di/Injectable.h"
#include "dao/FilesystemQueryLoader.h"
#include "util/Once.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, FilesystemQueryLoader)
BEEEON_OBJECT_CASTABLE(QueryLoader)
BEEEON_OBJECT_PROPERTY("rootPath", &FilesystemQueryLoader::setRootPath)
BEEEON_OBJECT_PROPERTY("extension", &FilesystemQueryLoader::setExtension)
BEEEON_OBJECT_PROPERTY("preprocessors", &FilesystemQueryLoader::addPreprocessor)
BEEEON_OBJECT_END(BeeeOn, FilesystemQueryLoader)

FilesystemQueryLoader::FilesystemQueryLoader()
{
}

FilesystemQueryLoader::~FilesystemQueryLoader()
{
}

string FilesystemQueryLoader::find(const string &key) const
{
	string query;
	Path p(keyAsFile(key));

	if (logger().debug()) {
		logger().debug("loading query " + key + " from " + p.toString(),
				__FILE__, __LINE__);
	}

	try {
		Poco::FileInputStream istr(p.toString());
		StreamCopier::copyToString(istr, query);
	} catch (const Exception &e) {
		throw NotFoundException("failed to read query " + key, e);
	}

	if (logger().trace()) {
		logger().trace("loaded query " + key + ":\n" + query,
				__FILE__, __LINE__);
	}

	return m_chain.process(query);
}

void FilesystemQueryLoader::setRootPath(const string &rootPath)
{
	m_rootPath = Path(rootPath);
	m_rootPath.makeDirectory();
}

void FilesystemQueryLoader::setExtension(const string &extension)
{
	m_extension = extension;
}

void FilesystemQueryLoader::addPreprocessor(Preprocessor::Ptr pp)
{
	m_chain.add(pp);
}

Path FilesystemQueryLoader::keyAsFile(const string& key) const
{
	StringTokenizer tokenizer(key, ".",
		StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
	size_t parts = tokenizer.count();

	Path result(m_rootPath);

	for (const auto &fragment : tokenizer) {
		if (parts == 1) // last part of path
			result.setFileName(fragment);
		else
			result.pushDirectory(fragment);

		parts--;
	}

	result.setExtension(m_extension);

	return result;
}

