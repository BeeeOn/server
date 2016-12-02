<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="schema.common.xsl" />

	<x:output method="text" />

	<x:template match="/">
		<x:apply-templates select="database" />
	</x:template>

	<x:template match="database">
		<x:variable name="name">
			<x:if test="@name">
				<x:value-of select="@name" />
			</x:if>
			<x:if test="@name">
				<x:value-of select="generate-id(.)" />
			</x:if>
		</x:variable>

		<x:text>digraph </x:text>
		<x:value-of select="$name" />
		<x:text> {&#xA;</x:text>
		<x:text>  graph [rankdir="LR"];&#xA;</x:text>
		<x:apply-templates select="table" mode="define" />
		<x:apply-templates select="table" mode="relations" />
		<x:text>}&#xA;</x:text>
	</x:template>

	<x:template match="table" mode="define">
		<x:text>  "</x:text>
		<x:call-template name="print-name" />
		<x:text>" [&#xA;</x:text>
		<x:text>    label="&lt;top&gt;</x:text>
		<x:call-template name="print-name" />

		<x:for-each select="column">
			<x:value-of select="concat(' | ', '&lt;', @name, '&gt; ', @name)" />
		</x:for-each>

		<x:text>"&#xA;</x:text>
		<x:text>    shape="record"</x:text>
		<x:text>  ];&#xA;</x:text>
	</x:template>

	<x:template match="database/table" mode="relations">
		<x:variable name="name">
			<x:call-template name="print-name" />
		</x:variable>

		<x:for-each select="foreign-key">
			<x:if test="@table and @local and @remote">
				<x:value-of select="concat('&quot;', $name, '&quot;')" />
				<x:text>:</x:text>
				<x:value-of select="@local" />
				<x:text> -> </x:text>
				<x:value-of select="concat('&quot;', @table, '&quot;')" />
				<x:value-of select="concat(':', @remote, ';')" />
			</x:if>
			<x:if test="not(@local) or not(@remote)">
				<x:for-each select="pair">
					<x:value-of select="concat('&quot;', $name, '&quot;')" />
					<x:text>:</x:text>
					<x:value-of select="@local" />
					<x:text> -> </x:text>
					<x:value-of select="concat('&quot;', ../@table, '&quot;')" />
					<x:value-of select="concat(':', @remote, ';')" />
				</x:for-each>
			</x:if>
		</x:for-each>
	</x:template>

</x:stylesheet>
