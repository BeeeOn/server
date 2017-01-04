<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="query.test.common.xsl" />

	<x:param name="engine" select="'postgre'" />

	<x:output method="text" />

	<x:template name="print-preamble">
		<x:text>\f ','&#xA;</x:text>
		<x:text>\a&#xA;</x:text>
		<x:text>\t on&#xA;</x:text>
		<x:text>\o '</x:text>
		<x:value-of select="concat(@for-database, '-', $engine, '-test.csv')" />
		<x:text>'&#xA;</x:text>
	</x:template>

	<x:template name="create-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:text>CREATE OR REPLACE FUNCTION fail_after_</x:text>
		<x:value-of select="$label" />
		<x:text>() RETURNS trigger AS $xxx$&#xA;</x:text>
		<x:text>  BEGIN INSERT INTO </x:text>
		<x:value-of select="$label" />
		<x:text>_should_fail (error) VALUES (</x:text>
		<x:call-template name="fail-query-message" />
		<x:text>); RETURN NULL; END; $xxx$ LANGUAGE plpgsql;&#xA;</x:text>

		<x:text>CREATE TRIGGER trigger_after_</x:text>
		<x:value-of select="$label" />
		<x:value-of select="concat(' AFTER ', $target, ' ON ', @table)" />
		<x:text> EXECUTE PROCEDURE fail_after_</x:text>
		<x:value-of select="$label" />
		<x:text>();&#xA;</x:text>
	</x:template>

	<x:template name="drop-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:text>DROP TRIGGER trigger_after_</x:text>
		<x:value-of select="$label" />
		<x:text> ON </x:text>
		<x:value-of select="@table" />
		<x:text>;&#xA;</x:text>

		<x:text>DROP FUNCTION fail_after_</x:text>
		<x:value-of select="$label" />
		<x:text>();&#xA;</x:text>
	</x:template>

	<x:template name="expect-header" />

	<x:template match="value" mode="csv">
		<x:value-of select="." />
	</x:template>

	<x:template match="/query-set">
		<x:call-template name="generate-sql-tests" />
		<x:call-template name="generate-expect-values" />
	</x:template>

</x:stylesheet>
