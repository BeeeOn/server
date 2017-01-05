<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="schema.postgre.xsl" />
	<x:import href="util.isql.xsl" />

	<x:output method="text" />

	<x:template match="/">
		<x:apply-templates select="database" mode="to-file">
			<x:with-param name="suffix" select="'-postgre-isql'" />
		</x:apply-templates>
	</x:template>

</x:stylesheet>
