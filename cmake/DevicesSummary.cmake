find_program(XSLTPROC NAMES xsltproc)
if(XSLTPROC)
	execute_process(COMMAND
		"${XSLTPROC}"
		"${CMAKE_CURRENT_SOURCE_DIR}/tools/devices_summary.xsl"
		"${CMAKE_CURRENT_SOURCE_DIR}/conf/devices.xml"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/supported_devices.md")
	set(SUPPORTED_DEVICES "INPUT += ${CMAKE_CURRENT_BINARY_DIR}/supported_devices.md")
endif()
