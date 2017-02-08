<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="query.test.common.xsl" />

	<x:param name="engine" select="'sqlite'" />

	<x:output method="text" />

	<x:template name="print-preamble">
		<x:text>.mode csv</x:text>
		<x:call-template name="new-line" />
		<x:text>.headers on</x:text>
		<x:call-template name="new-line" />
		<x:text>.output </x:text>
		<x:value-of select="concat(@for-database, '-', $engine, '-test.csv', $new.line)" />
		<x:text>PRAGMA foreign_keys = ON;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="create-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:text>CREATE TRIGGER trigger_after_</x:text>
		<x:value-of select="$label" />
		<x:value-of select="concat(' AFTER ', $target, ' ON ', @table)" />
		<x:text> BEGIN INSERT INTO </x:text>
		<x:value-of select="$label" />
		<x:text>_should_fail (error) VALUES (</x:text>
		<x:call-template name="fail-query-message" />
		<x:text>); END;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="drop-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:text>DROP TRIGGER trigger_after_</x:text>
		<x:value-of select="$label" />
		<x:text>;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template match="value[@type='real' or @type='double']" mode="csv">
		<x:variable name="is.integer" select="floor(number(.)) = number(.)" />

		<x:if test="$is.integer">
			<x:value-of select="format-number(number(.), '#.0')" />
		</x:if>
		<x:if test="not($is.integer)">
			<x:value-of select="." />
		</x:if>
	</x:template>

	<x:template match="/query-set">
		<x:call-template name="generate-sql-tests" />
		<x:call-template name="generate-expect-values" />
	</x:template>

</x:stylesheet>
