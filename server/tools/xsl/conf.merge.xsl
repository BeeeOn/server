<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:s="http://exslt.org/strings"
		extension-element-prefixes="s"
		version="1.0">

	<x:param name="conf.inputs" select="''" />

	<x:output method="xml" encoding="utf-8" />

	<x:template match="/">
		<x:copy>
			<x:apply-templates />
		</x:copy>
	</x:template>

	<x:template match="factory">
		<factory>
			<x:copy-of select="//factory/*" />

			<x:for-each select="s:split($conf.inputs, ' ')">
				<x:copy-of select="document(text(), .)//factory/*" />
			</x:for-each>
		</factory>
	</x:template>

	<x:template match="*">
		<x:copy>
			<x:apply-templates />
		</x:copy>
	</x:template>

</x:stylesheet>
