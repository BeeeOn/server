<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="schema.common.xsl" />

	<x:output method="text" />

	<x:template name="print-type">
		<x:call-template name="assert-has-type" />

		<x:choose>
			<x:when test="@type = 'int64'">
				<x:text>bigint</x:text>
			</x:when>
			<x:when test="@type = 'uuid'">
				<x:text>uuid</x:text>
			</x:when>
			<x:when test="@type = 'xml'">
				<x:text>xml</x:text>
			</x:when>
			<x:otherwise>
				<x:call-template name="print-type-common" />
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template name="sql-drop-table">
		<x:param name="name" />

		<x:text>DROP TABLE IF EXISTS </x:text>
		<x:value-of select="." />
		<x:text> CASCADE;&#xA;</x:text>
	</x:template>

	<x:template name="sql-drop-view">
		<x:param name="name" />

		<x:text>DROP VIEW IF EXISTS </x:text>
		<x:value-of select="." />
		<x:text> CASCADE;&#xA;</x:text>
	</x:template>

	<x:template match="/">
		<x:apply-templates select="database" mode="to-file">
			<x:with-param name="suffix" select="'-postgre'" />
		</x:apply-templates>
	</x:template>

</x:stylesheet>
