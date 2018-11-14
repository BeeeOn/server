function(TargetLink TARGET CORE LIBS)
	if (APPLE)
		# Apple's linker doesn't support --whole-archive.
		# Instead it uses -all_load.
		target_link_libraries(
			${TARGET}
			-Wl,--all_load
			${CORE}
			${LIBS})
	else ()
		# The -Wl,--whole-archive is necessary for all
		# libraries defining BEEEON_OBJECTs (see di/Injectable.h).
		target_link_libraries(
			${TARGET}
			-Wl,--whole-archive
			${CORE}
			-Wl,--no-whole-archive
			${LIBS})
	endif ()
endfunction(TargetLink)
