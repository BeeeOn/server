#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Path.h>

#include "dao/QueryLoader.h"
#include "util/PreprocessorChain.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief FilesystemQueryLoader is an implementation of the QueryLoader
 * that reads queries from files. Each query is expected to be stored
 * in a single file.
 *
 * Queries are identified by keys. A key is a dot-separated string.
 * The key is mapped into a file by replacing dots with path separators.
 * A file extension can be configured to be appended for the lookup.
 *
 * All queries are expected to be placed into a directory or a hierarchy
 * of directories. The lookup is always performed from the
 * configured root path.
 */
class FilesystemQueryLoader: public QueryLoader, Loggable {
public:
	FilesystemQueryLoader();
	virtual ~FilesystemQueryLoader();

	std::string find(const std::string &key) const;

	/**
	 * @brief Configure root path to denote where to load queries from.
	 * Example: for rootPath <code>~/src/</code> and key <code>database.server.select</code>,
	 * the resulted dir for lookup of the query would be <code>~/src/database/server</code>.
	 */
	void setRootPath(const std::string &rootPath);

	/**
	 * @brief Configure extension of the files containing queries.
	 *
	 * Given a key <code>database.server.select</code> and extension
	 * <code>sql</code>, the query in file
	 * <code>$rootPath/database/server/select.sql</code> would be loaded.
	 *
	 * If the extension is set to an empty string, no extension is
	 * considered. In that case,
	 * <code>$rootPath/database/server/select<code> would be loaded.
	 */
	void setExtension(const std::string &extension);

	/**
	 * @brief Add preprocessor into the internal preprocessor chain that would be
	 * used to preprocess the SQL queries when loading from the file system.
	 */
	void addPreprocessor(Preprocessor::Ptr pp);

private:
	/**
	 * Convert key <code>database.server.select</code> into the appropriate
	 * file path by considering the rootPath and extension.
	 */
	Poco::Path keyAsFile(const std::string& key) const;

private:
	Poco::Path m_rootPath;
	std::string m_extension;
	PreprocessorChain m_chain;
};

}

