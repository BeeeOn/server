find_program(MARKED_MAN NAMES marked-man)
find_program(RONN NAMES ronn)

if(MARKED_MAN)
	set(MD_MAN "${MARKED_MAN}")
	set(MD_MAN_ARGS "")
elseif(RONN)
	set(MD_MAN "${RONN}")
	set(MD_MAN_ARGS --roff --pipe)
endif()

if(MD_MAN)
	message(STATUS "generating man pages")
	file(GLOB MD_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/doc" ${CMAKE_CURRENT_SOURCE_DIR}/doc/*.md)
	file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man")

	foreach(md ${MD_FILES})
		message(STATUS "generating man from ${md}")
		string(REPLACE ".md" ".7" man "${md}")
		execute_process(COMMAND
			${MD_MAN} ${MD_MAN_ARGS} "${CMAKE_CURRENT_SOURCE_DIR}/doc/${md}"
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
			OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/man/beeeon-server-${man}")
	endforeach()

	install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man/"
		DESTINATION share/man/man7
		COMPONENT core)
else()
	message(STATUS "missing tool for man pages generation, skipping")
endif()
