<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="schema.common.xsl" />
	<x:import href="util.sqltypes.postgre.xsl" />

	<x:output method="text" />

	<x:param name="engine" select="'postgre'" />

	<x:template name="sql-drop-table">
		<x:param name="name" />

		<x:text>DROP TABLE IF EXISTS </x:text>
		<x:value-of select="." />
		<x:text> CASCADE;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="sql-drop-view">
		<x:param name="name" />

		<x:text>DROP VIEW IF EXISTS </x:text>
		<x:value-of select="." />
		<x:text> CASCADE;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="sql-drop-trigger">
		<x:param name="table" />
		<x:param name="name" />

		<x:text>DROP TRIGGER IF EXISTS </x:text>
		<x:value-of select="$name" />
		<x:text> ON </x:text>
		<x:value-of select="$table" />
		<x:text> CASCADE;</x:text>
		<x:call-template name="new-line" />
		<x:text>DROP FUNCTION IF EXISTS </x:text>
		<x:value-of select="concat($name, '_execute()')" />
		<x:text> CASCADE;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="prepare-trigger">
		<x:text>CREATE OR REPLACE FUNCTION </x:text>
		<x:call-template name="print-trigger-name" />
		<x:text>_execute()</x:text>
		<x:call-template name="new-line" />
		<x:text>     RETURNS TRIGGER language plpgsql AS $$</x:text>
		<x:call-template name="new-line" />
		<x:apply-templates select="execute/sql" mode="simple" />
		<x:text> $$;</x:text>
		<x:call-template name="new-line" />
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="for-each-statement-trigger">
		<x:text>FOR EACH STATEMENT</x:text>
	</x:template>

	<x:template match="triggers/*/execute">
		<x:text>EXECUTE PROCEDURE </x:text>
		<x:call-template name="print-trigger-name">
			<x:with-param name="context" select=".." />
		</x:call-template>
		<x:text>_execute()</x:text>;
	</x:template>

	<x:template name="create-trigger">
		<x:call-template name="prepare-trigger" />
		<x:text>CREATE TRIGGER </x:text>
		<x:call-template name="print-trigger-name" />
		<x:call-template name="new-line" />
		<x:apply-templates select="." />
	</x:template>

	<x:template match="/">
		<x:apply-templates select="database" mode="to-file">
			<x:with-param name="suffix" select="'-postgre'" />
		</x:apply-templates>
	</x:template>

</x:stylesheet>
