<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="query.test.common.xsl" />

	<x:param name="engine" select="'sqlite'" />

	<x:output method="text" />

	<x:template name="print-preamble">
		<x:text>.mode csv&#xA;</x:text>
		<x:text>.headers on&#xA;</x:text>
		<x:text>.output </x:text>
		<x:value-of select="concat(@for-database, '-', $engine, '-test.csv', '&#xA;')" />
		<x:text>PRAGMA foreign_keys = ON;&#xA;</x:text>
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
		<x:text>); END;&#xA;</x:text>
	</x:template>

	<x:template name="drop-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:text>DROP TRIGGER trigger_after_</x:text>
		<x:value-of select="$label" />
		<x:text>;&#xA;</x:text>
	</x:template>

	<x:template match="/query-set">
		<x:call-template name="generate-sql-tests" />
		<x:call-template name="generate-expect-values" />
	</x:template>

</x:stylesheet>
