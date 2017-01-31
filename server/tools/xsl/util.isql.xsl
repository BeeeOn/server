<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:s="http://exslt.org/strings"
		extension-element-prefixes="s"
		version="1.0">

	<x:param name="new.line" select="'&#xD;&#xA;'" />

	<x:template name="remove-cr-lf">
		<x:param name="text" select="''" />
		<x:param name="eol" select="''" />

		<x:variable name="result" select="s:replace($text, '&#xD;&#xA;', ' ')" />
		<x:if test="string-length(normalize-space($result)) &gt; 0">
			<x:value-of select="$result" />
			<x:value-of select="$eol" />
		</x:if>
	</x:template>

	<x:template match="database/table">
		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:call-template name="create-table" />
			</x:with-param>
			<x:with-param name="eol" select="$new.line" />
		</x:call-template>

		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:apply-templates select="triggers" />
			</x:with-param>
			<x:with-param name="eol" select="$new.line" />
		</x:call-template>
	</x:template>

	<x:template match="database/view">
		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:call-template name="create-view" />
			</x:with-param>
			<x:with-param name="eol" select="$new.line" />
		</x:call-template>

		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:apply-templates select="triggers" />
			</x:with-param>
			<x:with-param name="eol" select="$new.line" />
		</x:call-template>
	</x:template>

</x:stylesheet>
