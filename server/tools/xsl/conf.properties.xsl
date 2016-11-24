<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:str="http://exslt.org/strings"
		xmlns:c="http://exslt.org/common"
                extension-element-prefixes="str c"
		version="1.0">

	<x:import href="conf.params.xsl" />

	<x:variable name="properties.default"
			select="document('conf.params.xsl')/*/x:variable" />

	<!--
	Provide list of available property keys.
	Apply exslt:node-set to the result.
	-->
	<x:template name="properties-list-keys">
		<x:param name="prefix" select="'ini.'" />
		<x:param name="properties"
			select="$properties.default[starts-with(@name, $prefix)]" />

		<x:for-each select="$properties">
			<key><x:value-of select="substring-after(@name, $prefix)" /></key>
		</x:for-each>
	</x:template>

	<!--
	Return value for the available key. If there is no such
	property, return the key.
	-->
	<x:template name="property-value">
		<x:param name="prefix" select="'ini.'" />
		<x:param name="key" />
		<x:param name="properties"
			select="$properties.default[starts-with(@name, $prefix)]" />

		<x:variable name="property" select="$properties[@name = concat($prefix, $key)]" />

		<x:if test="count($property) = 0">
			<x:value-of select="$key" />
		</x:if>
		<x:if test="count($property) &gt; 0">
			<x:variable name="param" select="$property[position() = 1]" />
			<x:if test="$param/@select">
				<x:value-of select='translate($param/@select, "&apos;", "")' />
			</x:if>
			<x:if test="not($param/@select)">
				<x:value-of select="$param" />
			</x:if>
		</x:if>
	</x:template>

	<x:template name="apply-properties-recursive">
		<x:param name="prefix" select="'ini.'" />
		<x:param name="properties"
			select="$properties.default[starts-with(@name, $prefix)]" />
		<x:param name="keys" />
		<x:param name="subject" />

		<x:if test="count($keys) = 0">
			<x:value-of select="$subject" />
		</x:if>

		<x:if test="count($keys) &gt; 0">
			<x:variable name="key" select="$keys[position() = 1]" />
			<x:variable name="value">
				<x:call-template name="property-value">
					<x:with-param name="properties" select="$properties" />
					<x:with-param name="key" select="$key" />
				</x:call-template>
			</x:variable>

			<x:call-template name="apply-properties-recursive">
				<x:with-param name="properties" select="$properties" />
				<x:with-param name="keys" select="$keys[position() &gt; 1]" />
				<x:with-param name="subject"
					select="str:replace($subject, concat('${', $key, '}'), $value)" />
			</x:call-template>
		</x:if>
	</x:template>

	<!--
	Substitute properties like ${may.cool.property}
	in the given subject argument.
	-->
	<x:template name="apply-properties">
		<x:param name="prefix" select="'ini.'" />
		<x:param name="properties"
			select="$properties.default[starts-with(@name, $prefix)]" />
		<x:param name="subject" />

		<x:if test="$verbosity &gt; 2">
			<x:message>resolve <x:value-of select="$subject" /></x:message>
		</x:if>

		<x:variable name="keys">
			<x:call-template name="properties-list-keys">
				<x:with-param name="properties" select="$properties" />
			</x:call-template>
		</x:variable>

		<x:variable name="result">
			<x:call-template name="apply-properties-recursive">
				<x:with-param name="properties" select="$properties" />
				<x:with-param name="keys" select="c:node-set($keys)/key" />
				<x:with-param name="subject" select="$subject" />
			</x:call-template>
		</x:variable>

		<x:if test="$verbosity &gt; 1">
			<x:message>Resolved <x:value-of select="$subject" /> as <x:value-of select="$result" /></x:message>
		</x:if>

		<x:value-of select="$result" />
	</x:template>

</x:stylesheet>
