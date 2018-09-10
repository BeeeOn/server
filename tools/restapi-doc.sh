#! /bin/sh

# Usage:
#
#  $ tools/restapi-doc.sh > restapi.h
#
# or
#
#  $ ROOT=/home/me/sources/server tools/restapi-doc.sh > restapi.h
#
# The output contains C++ comments with Doxygen markups and it
# represents a single Doxygen page that can be easily included
# into a Doxygen project.
#
# Read INI file with mappings between actions defined in C++ sources
# and HTTP REST endpoints. Each mapping is then used to generate
# documentation of the particular endpoint.
#
# The INI file contains some more properties related to each endpoint.
# Some of them are automatically included into the resulted documentation.
#
# Documentation for each endpoint is extracted from the appropriate
# C++ method. Thus, we have to inspect C++ sources and find the
# mapped methods there. Only classes marked by the BEEEON_OBJECT_*
# macros can be inspected this way (otherwise we would have to
# understand C++ semantics much deeper). Each REST handler defines
# its methods via registerAction() template method and inherits from
# RestHandler which receives its group name.

parse_register_action()
{
	awk '
		/^BEEEON_OBJECT_BEGIN\(/ {
			count = split($0, part, /[\(,)] */)
			clazz = part[count - 1]
			current = ""

			for (i = 2; i < count - 1; ++i)
				current = current part[i] "::"

			if (namespace[clazz]) {
				if (namespace[clazz] != current)
					print "class " clazz " in 2 namespaces" >"/dev/stderr"
			}
			else {
				namespace[clazz] = current
			}
		}
		/RestHandler\("/ {
			sub(/^[^"]+"/, "")
			sub(/"[^"]+$/, "")
			group = $0
		}
		/registerAction</ {
			code = ""
		}
		/registerAction</, /);/ {
			sub(/^[ \t]+/, " ")
			code = code $0
		}
		/);/ {
			if (length(code) > 0) {
				# extract API call name
				name = code
				sub(/^[ \t]*registerAction[^(]+\([ \t]*"/, "", name)
				sub(/".*$/, "", name)

				# extract C++ method name
				call = code
				sub(/^[^&]*&/, "", call)
				sub(/,[ \t]*{.*$/, "", call)
				sub(/);$/, "", call)

				split(call, part, "::")
				if (namespace[part[1]])
					call = namespace[part[1]] call

				# extract API arguments
				args = code
				sub(/^[^{]*{/, "", args)
				sub(/}.*$/, "", args)
				sub(/^.*);$/, "", args)

				print group "." name ";" call ";" args
			}
			code = ""
		}
	' `find "${1}" -type f -readable`
}

restapi_get()
{
	local file="${1}"
	local action="${2}"
	local key="${3}"

	awk -F "=" -v ACTION="${action}" -v KEY="${key}" '
		$1 ~ "^api\\.default\\." KEY "[ \t]*$" {
			sub(/^[ \t]*/, "", $2)
			sub(/[ \t]*$/, "", $2)
			defval = $2
		}
		$1 ~ "^api\\.call\\." ACTION "\\." KEY "[ \t]*$" {
			sub(/^[ \t]*/, "", $2)
			sub(/[ \t]*$/, "", $2)
			val = $2
		}
		END {
			if (val)
				print val
			else
				print defval
		}' "${file}"
}

generate_docs()
{
	inifile="${1}/restui-api.ini"

	awk '/^ \*\// {next} {print}' "${1}/../doc/restui-api.inc"
	echo " *"

	while read line; do
		action=`awk -F ";" '{print $1}' <<< "${line}"`
		call=`awk -F ";" '{print $2 "(BeeeOn::RestFlow &)"}' <<< "${line}"`
		id=`tr '.' '_' <<< "${action}"`

		method=`restapi_get "${inifile}" "${action}" method`
		uri=`restapi_get "${inifile}" "${action}" uri`

		if [ -z "${uri}" ]; then
			echo " * \\section ${id} ${method} ${action} (internal)"
		else
			echo " * \\section ${id} ${method} ${uri}"
		fi

		echo " *"
		echo " * \\copybrief ${call}"
		echo " *"
		echo " * \\subsection ${id}_request Request"
		echo " *"

		session=`restapi_get "${inifile}" "${action}" session.required`
		if [ "${session}" == "yes" ]; then
			echo " * - session is required"
		fi

		input_size=`restapi_get "${inifile}" "${action}" input.maxsize`
		echo " * - maximal allowed input data size: ${input_size} B"
		echo " *"

		input_type=`restapi_get "${inifile}" "${action}" input.type`
		echo " * - Content-Type: ${input_type}"

		echo " *"
		echo " * \\subsection ${id}_response Response"
		echo " *"

		output_type=`restapi_get "${inifile}" "${action}" output.type`
		echo " * - Content-Type: ${output_type}"

		caching=`restapi_get "${inifile}" "${action}" caching`
		if [ "${caching}" == "no" ]; then
			echo " * - Cache-Control: public, no-cache"
		elif [ "${caching}" -gt 0 ]; then
			echo " * - Expires: {NOW} + ${caching}"
			echo " * - Cache-Control: max-age=${caching}, must-revalidate"
		fi

		echo " *"
		echo " * \\subsection ${id}_details Details"
		echo " *"
		echo " * \\copydetails ${call}"
		echo " * \\see ${call}"
	done

	echo " */"
}

test -z "${1}" && ROOT=`pwd` || ROOT="${1}"

parse_register_action "${ROOT}/src" | generate_docs "${ROOT}/conf"
