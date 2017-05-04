<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:template name="assert-has-type">
		<x:if test="not(@type)">
			<x:message terminate="yes">Missing type for <x:value-of select="local-name()" /></x:message>
		</x:if>
	</x:template>

	<x:template name="assert-has-size">
		<x:if test="not(@size)">
			<x:message terminate="yes">Missing size for <x:value-of select="local-name()" /></x:message>
		</x:if>
	</x:template>

	<x:template name="assert-has-precision">
		<x:if test="not(@precision)">
			<x:message terminate="yes">Missing precision for <x:value-of select="local-name()" /></x:message>
		</x:if>
	</x:template>

	<x:template name="assert-has-scale">
		<x:if test="not(@scale)">
			<x:message terminate="yes">Missing scale for <x:value-of select="local-name()" /></x:message>
		</x:if>
	</x:template>

	<x:template name="print-type-common">
		<x:call-template name="assert-has-type" />

		<x:choose>
			<x:when test="@type = 'integer'">
				<x:text>integer</x:text>
			</x:when>
			<x:when test="@type = 'smallint'">
				<x:text>smallint</x:text>
			</x:when>
			<x:when test="@type = 'bigint'">
				<x:text>bigint</x:text>
			</x:when>
			<x:when test="@type = 'character' or @type = 'char'">
				<x:text>character</x:text>
				<x:call-template name="assert-has-size" />
				<x:value-of select="concat('(', @size, ')')" />
			</x:when>
			<x:when test="@type = 'varchar' or @type = 'character varying'">
				<x:text>varchar</x:text>
				<x:call-template name="assert-has-size" />
				<x:value-of select="concat('(', @size, ')')" />
			</x:when>
			<x:when test="@type = 'binary'">
				<x:text>binary</x:text>
				<x:call-template name="assert-has-size" />
				<x:value-of select="concat('(', @size, ')')" />
			</x:when>
			<x:when test="@type = 'varbinary' or @type = 'binary varying'">
				<x:text>varbinary</x:text>
				<x:call-template name="assert-has-size" />
				<x:value-of select="concat('(', @size, ')')" />
			</x:when>
			<x:when test="@type = 'text'">
				<x:text>text</x:text>
			</x:when>
			<x:when test="@type = 'boolean' or @type = 'bool'">
				<x:text>boolean</x:text>
			</x:when>
			<x:when test="@type = 'real'">
				<x:text>real</x:text>
			</x:when>
			<x:when test="@type = 'float'">
				<x:text>float</x:text>
			</x:when>
			<x:when test="@type = 'double precision' or @type = 'double'">
				<x:text>double precision</x:text>
			</x:when>
			<x:when test="@type = 'numeric' or @type = 'decimal'">
				<x:value-of select="@type" />
				<x:call-template name="assert-has-precision" />
				<x:call-template name="assert-has-scale" />
				<x:value-of select="concat('(', @precision, ', ', @scale, ')')" />
			</x:when>
			<x:otherwise>
				<x:message terminate="yes">Unrecognized type <x:value-of select="concat(@type, ' for ', local-name())" /></x:message>
			</x:otherwise>
		</x:choose>
	</x:template>

</x:stylesheet>
