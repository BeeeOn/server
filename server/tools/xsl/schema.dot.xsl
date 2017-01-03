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
		<x:apply-templates select="table|view" mode="define" />
		<x:apply-templates select="table|view" mode="relations" />
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

	<x:template match="view" mode="define">
		<x:text>  "</x:text>
		<x:call-template name="print-name" />
		<x:text>" [&#xA;</x:text>
		<x:text>    style="filled" color="lightyellow" label="&lt;top&gt;</x:text>
		<x:call-template name="print-name" />

		<x:apply-templates select="map|join/map|sub-query" mode="define-entries" />

		<x:text>"&#xA;</x:text>
		<x:text>    shape="record"</x:text>
		<x:text>  ];&#xA;</x:text>

		<x:apply-templates select="join|sub-query" mode="define" />
	</x:template>

	<x:template name="print-map-ref-as">
		<x:if test="not(@as)">
			<x:value-of select="@ref" />
		</x:if>
		<x:if test="@as">
			<x:value-of select="@as" />
		</x:if>
	</x:template>

	<x:template match="view/map|view/join/map" mode="define-entries">
		<x:variable name="as">
			<x:call-template name="print-map-ref-as" />
		</x:variable>
		<x:value-of select="concat(' | ', '&lt;', $as, '&gt; ', $as)" />
	</x:template>

	<x:template match="view/sub-query" mode="define-entries">
		<x:variable name="as">
			<x:call-template name="print-map-ref-as" />
		</x:variable>
		<x:value-of select="concat(' | ', '&lt;', $as, '&gt; ', $as)" />
	</x:template>

	<x:template match="view/sub-query" mode="define">
		<x:text>  "</x:text>
		<x:value-of select="generate-id(.)" />
		<x:text>" [&#xA;</x:text>
		<x:text>    style="filled" color="lawngreen" label="sub-query"</x:text>
		<x:text>&#xA;</x:text>
		<x:text>  ];&#xA;</x:text>
	</x:template>

	<x:template name="print-join-base-on">
		<x:if test="@with-base">
			<x:value-of select="concat(@with-base, '.', @on-base)" />
		</x:if>
		<x:if test="not(@with-base)">
			<x:value-of select="concat(../@base, '.', @on-base)" />
		</x:if>
	</x:template>

	<x:template name="print-join-remote-on">
		<x:value-of select="concat(@remote, '.', @on-remote)" />
	</x:template>

	<x:template match="view/join" mode="define">
		<x:text>  "</x:text>
		<x:value-of select="generate-id(.)" />
		<x:text>" [&#xA;</x:text>
		<x:text>    style="filled" color="mintcream" label="&lt;top&gt;</x:text>
		<x:text>join on</x:text>

		<x:variable name="base.id">
			<x:call-template name="print-join-base-on" />
		</x:variable>

		<x:variable name="remote.id">
			<x:call-template name="print-join-remote-on" />
		</x:variable>

		<x:value-of select="concat(' | &lt;', $base.id, '&gt;', $base.id)" />
		<x:value-of select="concat(' | &lt;', $remote.id, '&gt;', $remote.id)" />

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

	<x:template match="database/view" mode="relations">
		<x:variable name="name">
			<x:call-template name="print-name" />
		</x:variable>

		<x:value-of select="concat('&quot;', $name, '&quot;')" />
		<x:text>:top</x:text>
		<x:text> -> </x:text>
		<x:value-of select="concat('&quot;', @base, '&quot;')" />
		<x:text>:top;</x:text>

		<x:for-each select="join">
			<x:value-of select="concat('&quot;', $name, '&quot;')" />
			<x:text>:top</x:text>
			<x:text> -> </x:text>
			<x:value-of select="generate-id(.)" />
			<x:text>:"</x:text>
			<x:call-template name="print-join-base-on" />
			<x:text>";</x:text>

			<x:value-of select="generate-id(.)" />
			<x:text>:"</x:text>
			<x:call-template name="print-join-remote-on" />
			<x:text>"</x:text>
			<x:text> -> </x:text>
			<x:value-of select="concat('&quot;', @remote, '&quot;')" />
			<x:text>:top;</x:text>
		</x:for-each>

		<x:for-each select="sub-query">
			<x:value-of select="concat('&quot;', ../@name, '&quot;')" />
			<x:text>:</x:text>
			<x:value-of select="@as" />
			<x:text> -> </x:text>
			<x:value-of select="generate-id(.)" />
			<x:text>;</x:text>

			<!-- compute connection from sub-query to dependent tables -->
			<x:call-template name="extract-table-and-view-names">
				<x:with-param name="text" select="sql" />
			</x:call-template>
		</x:for-each>
	</x:template>

	<!-- finalize sub-query relations -->
	<x:template match="view/@name|table/@name" mode="extracted-from-text">
		<x:param name="context" />

		<x:value-of select="generate-id($context)" />
		<x:text> -> </x:text>
		<x:value-of select="concat('&quot;', ., '&quot;')" />
		<x:text>:top;</x:text>
	</x:template>

</x:stylesheet>
