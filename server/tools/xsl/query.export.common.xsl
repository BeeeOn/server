<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:c="http://exslt.org/common"
		xmlns:d="http://exslt.org/dates-and-times"
                extension-element-prefixes="c d"
		version="1.0">

	<x:import href="util.codebase.xsl" />

	<x:param name="engine" select="'unknown'" />

	<x:template name="print-preamble">
		<x:text>; Auto-generated header&#xA;</x:text>
		<x:value-of select="concat('; ', d:date-time(), '&#xA;')" />
		<x:call-template name="print-codebase-version">
			<x:with-param name="prefix" select="'; '" />
			<x:with-param name="suffix" select="'&#xA;'" />
		</x:call-template>

		<x:value-of select="concat('[', @for-database, ']')" />
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template match="/">
		<x:apply-templates select="query-set" />
	</x:template>

	<x:template match="query-set">
		<c:document href="{@for-database}-{$engine}-queries.ini" method="text">
			<x:call-template name="print-preamble" />
			<x:apply-templates select="query" />
		</c:document>
	</x:template>

	<x:template match="query">
		<x:value-of select="concat(@id, ' = ')" />
		<x:apply-templates select="define" />
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template match="define">
		<x:apply-templates />
	</x:template>

	<x:template match="define/text()">
		<x:value-of select="normalize-space(.)" />
	</x:template>

	<x:template match="define/raw">
		<x:value-of select="." />
	</x:template>

	<x:template match="define/value">
		<x:text> ? </x:text>
	</x:template>

</x:stylesheet>
