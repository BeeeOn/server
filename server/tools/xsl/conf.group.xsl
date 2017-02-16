<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="conf.resolve.xsl" />

	<!--
	Recursively lookup the group by the given $class and by group/@pattern.
	-->
	<x:template name="group-for-class-recursive">
		<x:param name="class" />
		<x:param name="groups"
			select="document('conf.params.xsl')/*/x:param[@name='groups']/group" />

		<x:if test="count($groups) &gt; 0">
			<x:variable name="group" select="$groups[position() = 1]" />

			<x:if test="contains($class, $group/@pattern)">
				<x:copy-of select="$group" />
			</x:if>
			<x:if test="not(contains($class, $group/@pattern))">
				<x:call-template name="group-for-class-recursive">
					<x:with-param name="class" select="$class" />
					<x:with-param name="groups" select="$groups[position() &gt; 1]" />
				</x:call-template>
			</x:if>
		</x:if>
	</x:template>

	<!--
	Return node (apply exslt:node-set) group as specified
	in the global $groups param selected by applying group/@pattern
	to the @class name of this element.
	-->
	<x:template name="group-for-class">
		<x:param name="class" select="@class" />
		<x:param name="groups"
			select="document('conf.params.xsl')/*/x:param[@name='groups']/group" />

		<x:call-template name="group-for-class-recursive">
			<x:with-param name="class">
				<x:call-template name="resolve-value">
					<x:with-param name="value" select="$class" />
				</x:call-template>
			</x:with-param>
			<x:with-param name="groups" select="$groups" />
		</x:call-template>
	</x:template>

	<x:template match="instance" mode="in-group">
		<x:param name="pattern" />
		<x:param name="ignore" />

		<x:variable name="name">
			<x:call-template name="print-simple-class" />
		</x:variable>

		<x:if test="not(contains($ignore, $name))">
			<x:if test="contains($name, $pattern)">
				<instance><x:value-of select="$name" /></instance>
			</x:if>
		</x:if>
	</x:template>

	<!--
	Group all instances by applying group/@pattern.
	Return nodes (apply exslt:node-set) like:

		<group name="...">
			<instance>...</instance>
			<instance>...</instance>
		</group>
		<group name="...">
			...
		</group>
		...
	-->
	<x:template name="group-instances">
		<x:param name="instances" select="//factory/instance" />
		<x:param name="ignore" select="''" />
		<x:param name="groups"
			select="document('conf.params.xsl')/*/x:param[@name='groups']/group" />

		<x:if test="count($groups) &gt; 0">
			<x:variable name="in.group">
				<x:for-each select="$groups[position() = 1]">
					<x:apply-templates select="$instances" mode="in-group">
						<x:with-param name="pattern" select="@pattern" />
						<x:with-param name="ignore" select="$ignore" />
					</x:apply-templates>
				</x:for-each>
			</x:variable>

			<group name="{$groups[position() = 1]/@pattern}">
				<x:copy-of select="$in.group" />
			</group>

			<x:call-template name="group-instances">
				<x:with-param name="instances" select="$instances" />
				<x:with-param name="ignore" select="concat($ignore, ' ', $in.group)" />
				<x:with-param name="groups" select="$groups[position() &gt; 1]" />
			</x:call-template>
		</x:if>
	</x:template>

</x:stylesheet>
