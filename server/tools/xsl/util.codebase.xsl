<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:param name="codebase.version"></x:param>

	<x:variable name="codebase.version.known"
			select="string-length($codebase.version) &gt; 0" />

	<x:template name="print-codebase-version">
		<x:param name="prefix" select="''" />
		<x:param name="suffix" select="''" />

		<x:if test="$codebase.version.known">
			<x:value-of select="concat($prefix, $codebase.version, $suffix)" />
		</x:if>
	</x:template>

</x:stylesheet>
