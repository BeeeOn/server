<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:c="http://exslt.org/common"
		xmlns:d="http://exslt.org/dates-and-times"
                extension-element-prefixes="c d"
		version="1.0">

	<x:import href="util.codebase.xsl" />

	<x:template name="print-header">
		<x:text>-- Auto-generated header&#xA;</x:text>
		<x:value-of select="concat('-- ', d:date-time(), '&#xA;')" />

		<x:call-template name="print-codebase-version">
			<x:with-param name="prefix" select="'-- '" />
			<x:with-param name="suffix" select="'&#xA;'" />
		</x:call-template>

		<x:if test="@name">
			<x:text>-- Database: </x:text>
			<x:value-of select="@name" />
			<x:text>&#xA;</x:text>
		</x:if>
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template name="print-name">
		<x:param name="context" select="." />

		<x:if test="not($context/@name)">
			<x:message terminate="yes">Missing name for <x:value-of select="local-name($context)" /></x:message>
		</x:if>

		<x:value-of select="$context/@name" />
	</x:template>

	<x:template name="print-default">
		<x:if test="@default">
			<x:text> DEFAULT </x:text>
			<x:value-of select="@name" />
		</x:if>
	</x:template>

	<x:template name="print-not-null">
		<x:choose>
			<x:when test="@not-null = 'yes'">
				<x:text> NOT NULL</x:text>
			</x:when>
			<x:when test="@not-null = 'y'">
				<x:text> NOT NULL</x:text>
			</x:when>
			<x:when test="@not-null = 'true'">
				<x:text> NOT NULL</x:text>
			</x:when>
			<x:when test="@not-null = 't'">
				<x:text> NOT NULL</x:text>
			</x:when>
		</x:choose>
	</x:template>

	<x:template name="assert-has-ref">
		<x:if test="not(@ref)">
			<x:message terminate="yes">Missing ref for <x:value-of select="local-name()" /></x:message>
		</x:if>
	</x:template>

	<x:template name="assert-has-size">
		<x:if test="not(@size)">
			<x:message terminate="yes">Missing size for <x:value-of select="local-name()" /></x:message>
		</x:if>
	</x:template>

	<x:template name="assert-has-type">
		<x:if test="not(@type)">
			<x:message terminate="yes">Missing type for <x:value-of select="local-name()" /></x:message>
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

	<x:template name="assert-has-valid-ref">
		<x:if test="not(@ref) and not(ref)">
			<x:message terminate="yes">Missing ref for <x:value-of select="local-name()" /></x:message>
		</x:if>
		<x:if test="@ref and ref">
			<x:message>@ref is preferred over ref for <x:value-of select="local-name()" /></x:message>
		</x:if>
	</x:template>

	<x:template name="print-local-list">
		<x:param name="context" select="." />

		<x:if test="$context/@local">
			<x:value-of select="$context/@local" />
		</x:if>
		<x:if test="$context/pair/@local and not($context/@local)">
			<x:for-each select="$context/pair/@local">
				<x:value-of select="." />
				<x:call-template name="comma-space-if-not-last" />
			</x:for-each>
		</x:if>
	</x:template>

	<x:template name="print-remote-list">
		<x:param name="context" select="." />

		<x:if test="$context/@remote">
			<x:value-of select="$context/@remote" />
		</x:if>
		<x:if test="$context/pair/@remote and not($context/@remote)">
			<x:for-each select="$context/pair/@remote">
				<x:value-of select="." />
				<x:call-template name="comma-space-if-not-last" />
			</x:for-each>
		</x:if>
	</x:template>

	<x:template name="print-ref-list">
		<x:if test="@ref">
			<x:value-of select="@ref" />
		</x:if>
		<x:if test="ref and not(@ref)">
			<x:for-each select="ref">
				<x:value-of select="." />
				<x:call-template name="comma-space-if-not-last" />
			</x:for-each>
		</x:if>
	</x:template>

	<x:template name="print-table">
		<x:if test="not(@table)">
			<x:message terminate="yes">Missing table for <x:value-of select="local-name()" /></x:message>
		</x:if>

		<x:value-of select="@table" />
	</x:template>

	<x:template name="print-primary-key-name">
		<x:if test="@name">
			<x:value-of select="@name" />
		</x:if>

		<x:if test="not(@name)">
			<x:value-of select="concat(../@name, '_pk')" />
		</x:if>
	</x:template>

	<x:template name="print-foreign-key-name">
		<x:if test="@name">
			<x:value-of select="@name" />
		</x:if>

		<x:if test="not(@name)">
			<x:value-of select="concat(../@name, '_fk')" />
		</x:if>
	</x:template>

	<x:template name="print-unique-name">
		<x:if test="@name">
			<x:value-of select="@name" />
		</x:if>

		<x:if test="not(@name)">
			<x:value-of select="concat(../@name, '_unique')" />
		</x:if>
	</x:template>

	<x:template name="print-check-name">
		<x:if test="@name">
			<x:value-of select="@name" />
		</x:if>

		<x:if test="not(@name)">
			<x:value-of select="concat(../@name, '_check_', generate-id(.))" />
		</x:if>
	</x:template>

	<x:template name="if-not-last">
		<x:param name="text" />

		<x:if test="position() &lt; last()">
			<x:value-of select="$text" />
		</x:if>
	</x:template>

	<x:template name="comma-space-if-not-last">
		<x:call-template name="if-not-last">
			<x:with-param name="text" select="', '" />
		</x:call-template>
	</x:template>

	<x:template name="comma-if-not-last">
		<x:call-template name="if-not-last">
			<x:with-param name="text" select="','" />
		</x:call-template>
	</x:template>

	<x:template name="comma-if-not-last-eol">
		<x:call-template name="comma-if-not-last" />
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template name="drop-tables-recursive">
		<x:param name="dropped" select="''" />
		<x:param name="tables" select="/database/table/@name" />

		<x:if test="count($tables) &gt; 0">
			<x:variable name="table" select="$tables[position() = 1]" />

			<x:variable name="dependencies" select="$table/foreign-key/@table" />
			<x:for-each select="$dependencies">
				<x:call-template name="drop-tables">
					<x:variable name="current" select="position()" />

					<x:with-param name="dropped">
						<x:value-of select="$dropped" />

						<!-- add all previously dropped in this loop -->
						<x:for-each select="$dependencies[position() &lt; $current]">
							<x:value-of select="concat(' ', .)" />
						</x:for-each>
					</x:with-param>
					<x:with-param name="tables" select="$table" />
				</x:call-template>
			</x:for-each>

			<x:if test="not(contains(concat(' ', $dropped, ' '), concat(' ', $table, ' ')))">
				<table><x:value-of select="$table" /></table>
			</x:if>

			<x:call-template name="drop-tables-recursive">
				<x:with-param name="dropped">
					<x:value-of select="$dropped" />
					<x:for-each select="$dependencies">
						<x:value-of select="concat(' ', .)" />
					</x:for-each>
					<x:value-of select="concat(' ', $table)" />
				</x:with-param>
				<x:with-param name="tables" select="$tables[position() &gt; 1]" />
			</x:call-template>
		</x:if>
	</x:template>

	<x:template name="drop-tables">
		<x:variable name="tables-to-drop">
			<x:call-template name="drop-tables-recursive" />
		</x:variable>

		<x:for-each select="c:node-set($tables-to-drop)/table">
			<x:sort select="position()" data-type="number" order="descending" />
			<x:text>DROP TABLE IF EXISTS </x:text>
			<x:value-of select="." />
			<x:text>;&#xA;</x:text>
		</x:for-each>
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

	<x:template match="database" mode="to-file">
		<x:param name="suffix" select="''" />
		<x:choose>
			<x:when test="@name">
				<c:document href="{@name}{$suffix}.sql" method="text" encoding="utf-8">
					<x:apply-templates select="." />
				</c:document>
			</x:when>
			<x:otherwise>
				<x:message>No name of <x:value-of select="position()" />-th database, print to stdout</x:message>
				<x:apply-templates select="." />
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template match="database">
		<x:call-template name="print-header" />
		<x:call-template name="drop-tables" />
		<x:text>&#xA;</x:text>
		<x:apply-templates select="table" />
	</x:template>

	<x:template match="database/table">
		<x:text>CREATE TABLE </x:text>
		<x:call-template name="print-name" />
		<x:text> (&#xA;</x:text>
		<x:apply-templates select="column|primary-key|foreign-key|unique|check" />
		<x:text>);&#xA;</x:text>
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template match="database/table/column">
		<x:text>    </x:text>
		<x:call-template name="print-name" />
		<x:text> </x:text>
		<x:call-template name="print-type" />
		<x:call-template name="print-not-null" />
		<x:call-template name="print-default" />
		<x:call-template name="comma-if-not-last-eol" />
	</x:template>

	<x:template match="table/primary-key">
		<x:text>    </x:text>
		<x:text>CONSTRAINT </x:text>
		<x:call-template name="print-primary-key-name" />
		<x:text> PRIMARY KEY (</x:text>
		<x:call-template name="assert-has-valid-ref" />
		<x:call-template name="print-ref-list" />
		<x:text>)</x:text>
		<x:call-template name="comma-if-not-last-eol" />
	</x:template>

	<x:template match="table/foreign-key">
		<x:text>    </x:text>
		<x:text>CONSTRAINT </x:text>
		<x:call-template name="print-foreign-key-name" />
		<x:text> FOREIGN KEY (</x:text>
		<x:call-template name="print-local-list" />
		<x:text>) REFERENCES </x:text>
		<x:call-template name="print-table" />
		<x:text> (</x:text>
		<x:call-template name="print-remote-list" />
		<x:text>)</x:text>
		<x:call-template name="comma-if-not-last-eol" />
	</x:template>

	<x:template match="table/unique">
		<x:text>    </x:text>
		<x:text>CONSTRAINT </x:text>
		<x:call-template name="print-unique-name" />
		<x:text> UNIQUE (</x:text>
		<x:call-template name="assert-has-valid-ref" />
		<x:call-template name="print-ref-list" />
		<x:text>)</x:text>
		<x:call-template name="comma-if-not-last-eol" />
	</x:template>

	<x:template match="table/check">
		<x:text>    </x:text>
		<x:text>CONSTRAINT </x:text>
		<x:call-template name="print-check-name" />
		<x:text> CHECK (</x:text>
		<x:value-of select="@expression" />
		<x:text>)</x:text>
		<x:call-template name="comma-if-not-last-eol" />
	</x:template>

</x:stylesheet>
