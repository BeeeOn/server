<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:include href="conf.params.xsl" />
	<x:include href="conf.properties.xsl" />

	<x:template name="resolve-value">
		<x:param name="value" select="''" />

		<x:call-template name="apply-properties">
			<x:with-param name="subject" select="$value" />
		</x:call-template>
	</x:template>

	<x:template name="print-name">
		<x:call-template name="resolve-value">
			<x:with-param name="value" select="@name" />
		</x:call-template>
	</x:template>

	<x:template name="print-ref">
		<x:call-template name="resolve-value">
			<x:with-param name="value" select="@ref" />
		</x:call-template>
	</x:template>

	<x:template name="print-class">
		<x:call-template name="resolve-value">
			<x:with-param name="value" select="@class" />
		</x:call-template>
	</x:template>

	<x:template name="print-simple-class-recursive">
		<x:param name="class" select="@class" />

		<x:if test="contains($class, '::')">
			<x:call-template name="print-simple-class-recursive">
				<x:with-param name="class"
					select="substring-after($class, '::')" />
			</x:call-template>
		</x:if>

		<x:if test="not(contains($class, '::'))">
			<x:value-of select="$class" />
		</x:if>
	</x:template>

	<x:template name="print-simple-class">
		<x:call-template name="print-simple-class-recursive">
			<x:with-param name="class">
				<x:call-template name="print-class" />
			</x:with-param>
		</x:call-template>
	</x:template>

	<!--
	We have reached the instance, resolution is complete.
	-->
	<x:template match="instance" mode="resolve-class">
		<x:call-template name="print-class" />
	</x:template>

	<!--
	Traverse through aliases until instance is reached.
	-->
	<x:template match="alias" mode="resolve-class">
		<x:variable name="ref">
			<x:call-template name="print-ref" />
		</x:variable>

		<x:for-each select="//services/instance|//services/alias">
			<x:variable name="name">
				<x:call-template name="print-name" />
			</x:variable>

			<x:if test="$name = $ref">
				<x:apply-templates select="." mode="resolve-class" />
			</x:if>
		</x:for-each>
	</x:template>

	<x:template name="print-alias-class">
		<x:variable name="class">
			<x:apply-templates select="." mode="resolve-class" />
		</x:variable>

		<x:if test="string-length($class) = 0">
			<x:message terminate="yes">No such alias resolved for <x:value-of select="@name" /></x:message>
		</x:if>

		<x:value-of select="$class" />
	</x:template>

	<x:template name="print-alias-simple-class">
		<x:call-template name="print-simple-class-recursive">
			<x:with-param name="class">
				<x:call-template name="print-alias-class" />
			</x:with-param>
		</x:call-template>
	</x:template>

	<!--
	We have reached the instance, resolution is complete.
	-->
	<x:template match="instance" mode="resolve-name">
		<x:call-template name="print-name" />
	</x:template>

	<!--
	Traverse through aliases until instance is reached.
	-->
	<x:template match="alias" mode="resolve-name">
		<x:variable name="ref">
			<x:call-template name="print-ref" />
		</x:variable>

		<x:for-each select="//services/instance|//services/alias">
			<x:variable name="name">
				<x:call-template name="print-name" />
			</x:variable>

			<x:if test="$name = $ref">
				<x:apply-templates select="." mode="resolve-name" />
			</x:if>
		</x:for-each>
	</x:template>

	<!--
	In case the ref points to an alias, we have to
	resolve that alias until an instance is found.
	-->
	<x:template name="print-name-of-ref">
		<x:param name="ref">
			<x:call-template name="print-ref" />
		</x:param>

		<x:variable name="result">
			<x:for-each select="//services/instance|//services/alias">
				<x:variable name="name">
					<x:call-template name="print-name" />
				</x:variable>

				<x:if test="$name = $ref">
					<x:apply-templates select="." mode="resolve-name" />
				</x:if>
			</x:for-each>
		</x:variable>
	
		<x:if test="string-length($result) = 0">
			<x:message terminate="yes">No such alias resolved for <x:value-of select="@name" /></x:message>
		</x:if>

		<x:value-of select="$result" />	
	</x:template>

	<x:template name="count-of-referrers">
		<x:param name="name">
			<x:call-template name="print-name" />
		</x:param>

		<x:variable name="result">
			<x:for-each select="//*[@ref]">
				<x:variable name="ref">
					<x:call-template name="print-ref" />
				</x:variable>

				<x:if test="$name = $ref">
					<x:text>y</x:text>
				</x:if>
			</x:for-each>
		</x:variable>

		<x:value-of select="string-length($result)" />
	</x:template>

	<x:template name="refs-count">
		<x:value-of select="count(*[@ref])" />
	</x:template>

</x:stylesheet>
