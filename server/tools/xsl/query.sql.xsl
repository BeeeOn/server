<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:param name="engine" select="'unknown'" />

	<x:template match="sql" mode="simple">
		<x:if test="not(@engine) or $engine = @engine">
			<x:choose>
				<x:when test="@normalize = 'disable'">
					<x:value-of select="." />
				</x:when>
				<x:otherwise>
					<x:value-of select="normalize-space(.)" />
				</x:otherwise>
			</x:choose>
			<x:text>;&#xA;</x:text>
		</x:if>
	</x:template>

	<x:template match="define" mode="sql-direct">
		<x:param name="args" />

		<x:if test="not(@engine) or $engine = @engine">
			<x:apply-templates select="text()|value" mode="sql-direct">
				<x:with-param name="args" select="$args" />
			</x:apply-templates>

			<x:text>;&#xA;</x:text>
		</x:if>
	</x:template>

	<x:template match="define/text()" mode="sql-direct">
		<x:param name="args" />

		<x:if test="position() &gt; 1">
			<x:text> </x:text>
		</x:if>

		<x:value-of select="normalize-space(.)" />

		<x:if test="position() != last()">
			<x:text> </x:text>
		</x:if>
	</x:template>

	<x:template match="define/raw" mode="sql-direct">
		<x:param name="args" />
		<x:value-of select="." />
	</x:template>

	<x:template match="define/value" mode="sql-direct">
		<x:param name="args" />

		<x:variable name="name" select="@name" />
		<x:variable name="arg" select="$args[@name = $name]" />

		<x:if test="not($arg)">
			<x:message terminate="yes">Missing arg with name '<x:value-of select="$name" />'</x:message>
		</x:if>

		<x:value-of select="$arg" />
	</x:template>

	<x:template match="define" mode="export">
		<x:apply-templates mode="export" />
	</x:template>

	<x:template match="define/text()" mode="export">
		<x:value-of select="normalize-space(.)" />
	</x:template>

	<x:template match="define/raw" mode="export">
		<x:value-of select="." />
	</x:template>

	<x:template match="define/value" mode="export">
		<x:text> ? </x:text>
	</x:template>

</x:stylesheet>
