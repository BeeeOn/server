<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="query.test.common.xsl" />

	<x:param name="engine" select="'postgre'" />

	<x:output method="text" />

	<x:template name="print-preamble">
		<x:text>\f ','</x:text>
		<x:call-template name="new-line" />
		<x:text>\a</x:text>
		<x:call-template name="new-line" />
		<x:text>\t on</x:text>
		<x:call-template name="new-line" />
		<x:text>\o '</x:text>
		<x:value-of select="concat(@for-database, '-', $engine, '-test.csv')" />
		<x:text>'</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="create-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:text>CREATE OR REPLACE FUNCTION fail_after_</x:text>
		<x:value-of select="$label" />
		<x:text>() RETURNS trigger AS $xxx$</x:text>
		<x:call-template name="new-line" />
		<x:text>  BEGIN INSERT INTO </x:text>
		<x:value-of select="$label" />
		<x:text>_should_fail (error) VALUES (</x:text>
		<x:call-template name="fail-query-message" />
		<x:text>); RETURN NULL; END; $xxx$ LANGUAGE plpgsql;</x:text>
		<x:call-template name="new-line" />

		<x:text>CREATE TRIGGER trigger_after_</x:text>
		<x:value-of select="$label" />
		<x:value-of select="concat(' AFTER ', $target, ' ON ', @table)" />
		<x:text> EXECUTE PROCEDURE fail_after_</x:text>
		<x:value-of select="$label" />
		<x:text>();</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="drop-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:text>DROP TRIGGER trigger_after_</x:text>
		<x:value-of select="$label" />
		<x:text> ON </x:text>
		<x:value-of select="@table" />
		<x:text>;</x:text>
		<x:call-template name="new-line" />

		<x:text>DROP FUNCTION fail_after_</x:text>
		<x:value-of select="$label" />
		<x:text>();</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="print-csv-header" />

	<x:template name="csv-quote">
		<x:param name="value" select="." />
		<x:value-of select="$value" />
	</x:template>

	<x:template match="/query-set">
		<x:call-template name="generate-sql-tests" />
		<x:call-template name="generate-expect-values" />
	</x:template>

</x:stylesheet>
