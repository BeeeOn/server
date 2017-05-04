<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="schema.common.xsl" />
	<x:import href="util.sqltypes.sqlite.xsl" />

	<x:output method="text" />

	<x:param name="engine" select="'sqlite'" />

	<x:template match="/">
		<x:apply-templates select="database" mode="to-file">
			<x:with-param name="suffix" select="'-sqlite'" />
		</x:apply-templates>
	</x:template>

</x:stylesheet>
