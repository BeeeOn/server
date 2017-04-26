<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="util.sqltypes.xsl" />

	<x:template name="print-type">
		<x:call-template name="assert-has-type" />

		<x:choose>
			<x:when test="@type = 'int64'">
				<x:text>bigint</x:text>
			</x:when>
			<x:when test="@type = 'uuid'">
				<x:text>uuid</x:text>
			</x:when>
			<x:when test="@type = 'xml'">
				<x:text>xml</x:text>
			</x:when>
			<x:otherwise>
				<x:call-template name="print-type-common" />
			</x:otherwise>
		</x:choose>
	</x:template>

</x:stylesheet>
