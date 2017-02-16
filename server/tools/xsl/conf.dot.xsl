<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:c="http://exslt.org/common"
		extension-element-prefixes="c"
		version="1.0">

	<x:import href="conf.resolve.xsl" />
	<x:import href="conf.group.xsl" />

	<x:output method="text" encoding="utf-8" />
	<x:strip-space elements="*" />

	<x:template match="/">
		<x:text>digraph factory {&#xA;</x:text>
		<x:text>  graph [rankdir="LR"];&#xA;</x:text>
		<x:apply-templates match="//factory" />
		<x:text>}&#xA;</x:text>
	</x:template>

	<x:template match="factory">
		<x:apply-templates select="instance" mode="define" />
		<x:apply-templates select="instance" mode="deps" />
		<x:text>  {rank=same;</x:text>
		<x:apply-templates select="instance" mode="group-unused" />
		<x:text>}&#xA;</x:text>
	</x:template>

	<x:template match="group" mode="dot-colors">
		<x:if test="@color">
			<x:text>    </x:text>
			<x:value-of select="concat('color=&quot;', @color, '&quot;&#xA;')" />
		</x:if>
		<x:if test="@fillcolor">
			<x:text>    </x:text>
			<x:text>style="filled" </x:text>
			<x:value-of select="concat('color=&quot;', @fillcolor, '&quot;&#xA;')" />
		</x:if>
		<x:if test="@fontcolor">
			<x:text>    </x:text>
			<x:value-of select="concat('fontcolor=&quot;', @fontcolor, '&quot;&#xA;')" />
		</x:if>
	</x:template>

	<x:template name="seems-unused">
		<x:variable name="count.of.referrers">
			<x:call-template name="count-of-referrers" />
		</x:variable>
		<x:variable name="refs.count">
			<x:call-template name="refs-count" />
		</x:variable>

		<x:if test="$count.of.referrers = 0 and $refs.count = 0">
			<x:text>y</x:text>
		</x:if>
	</x:template>

	<x:template match="instance" mode="define">
		<x:variable name="seems.unused">
			<x:call-template name="seems-unused" />
		</x:variable>

		<x:if test="(not($hide.unused) or $seems.unused != 'y')">
			<x:apply-templates select="." mode="do-define" />
		</x:if>
	</x:template>

	<x:template match="instance" mode="do-define">
		<x:variable name="class">
			<x:call-template name="print-simple-class" />
		</x:variable>

		<x:text>  "</x:text>
		<x:call-template name="print-name" />
		<x:text>" [&#xA;</x:text>
		<x:text>    label="&lt;top&gt; </x:text>

		<!-- <top> name\nclass | <p0> @name | <p1> @name | ... -->
		<x:call-template name="print-name" />
		<x:value-of select="concat('\nclass ', $class)" />
		<x:for-each select="set[@ref]|add[@ref]">
			<x:value-of select="concat(' | &lt;', @name, position(), '&gt; ', @name)" />
		</x:for-each>
		<x:for-each select="set[not(@ref)]|add[not(@ref)]">
			<x:value-of select="concat(' | &lt;', @name, '&gt; ', @name)" />
		</x:for-each>
		<x:text>"&#xA;</x:text>
		<x:text>    shape="record"&#xA;</x:text>

		<x:variable name="group">
			<x:call-template name="group-for-class">
				<x:with-param name="class" select="$class" />
			</x:call-template>
		</x:variable>
		<x:apply-templates select="c:node-set($group)/group" mode="dot-colors" />

		<x:text>  ];&#xA;</x:text>
	</x:template>

	<x:template match="instance" mode="deps">
		<x:variable name="name">
			<x:call-template name="print-name" />
		</x:variable>

		<x:for-each select="set[@ref]|add[@ref]">
			<x:variable name="ref">
				<x:call-template name="print-name-of-ref" />
			</x:variable>

			<x:text>  </x:text>
			<x:value-of select="concat('&quot;', $name, '&quot;:')" />
			<x:call-template name="print-name" /><x:value-of select="position()" />
			<x:text> -> </x:text>
			<x:value-of select="concat('&quot;', $ref, '&quot;:top')" />
			<x:text>;&#xA;</x:text>
		</x:for-each>
	</x:template>

</x:stylesheet>
