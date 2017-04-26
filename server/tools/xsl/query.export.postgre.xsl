<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="query.export.common.xsl" />
	<x:import href="util.sqltypes.postgre.xsl" />

	<x:output method="text" />

	<x:param name="engine" select="'postgre'" />

	<!-- always perform explicit cast -->
	<x:template match="define/value" mode="export">
		<x:text> ?::</x:text>
		<x:call-template name="print-type" />
		<x:text> </x:text>
	</x:template>

</x:stylesheet>
