<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:c="http://exslt.org/common"
		xmlns:s="http://exslt.org/strings"
		xmlns:d="http://exslt.org/dates-and-times"
		extension-element-prefixes="c s d"
		version="1.0">

	<x:import href="util.codebase.xsl" />

	<x:param name="new.line" select="'&#xA;'" />

	<x:template name="new-line">
		<x:value-of select="$new.line" />
	</x:template>

	<x:template name="print-header">
		<x:text>-- Auto-generated header</x:text>
		<x:call-template name="new-line" />
		<x:value-of select="concat('-- ', d:date-time(), $new.line)" />

		<x:call-template name="print-codebase-version">
			<x:with-param name="prefix" select="'-- '" />
			<x:with-param name="suffix" select="$new.line" />
		</x:call-template>

		<x:if test="@name">
			<x:text>-- Database: </x:text>
			<x:value-of select="@name" />
			<x:call-template name="new-line" />
		</x:if>
		<x:call-template name="new-line" />
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
			<x:value-of select="@default" />
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
			<x:value-of select="concat(../@name, '_', @table, '_fk')" />
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

	<x:template name="print-remote">
		<x:if test="not(@remote)">
			<x:message terminate="yes">Missing remote for <x:value-of select="local-name()" /></x:message>
		</x:if>

		<x:value-of select="@remote" />
	</x:template>

	<x:template name="print-base">
		<x:if test="not(@base)">
			<x:message terminate="yes">Missing base for <x:value-of select="local-name()" /></x:message>
		</x:if>

		<x:value-of select="@base" />
	</x:template>

	<x:template name="print-view-map-ref-as">
		<x:param name="context" />

		<x:if test="not(@ref)">
			<x:message terminate="yes">Missing ref of map <x:value-of select="position()" /> of view <x:value-of select="@name" /></x:message>
		</x:if>

		<x:variable name="as">
			<x:if test="@as">
				<x:value-of select="@as" />
			</x:if>
			<x:if test="not(@as)">
				<x:value-of select="@ref" />
			</x:if>
		</x:variable>

		<x:value-of select="concat($context, '.', @ref, ' AS ', $as)" />
	</x:template>

	<x:template name="print-view-map">
		<x:param name="context" />

		<x:text>        </x:text>

		<x:call-template name="print-view-map-ref-as">
			<x:with-param name="context" select="$context" />
		</x:call-template>

		<x:if test="position() != last()">
			<x:text>,</x:text>
			<x:call-template name="new-line" />
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

	<x:template name="comma-eol-if-not-last">
		<x:call-template name="if-not-last">
			<x:with-param name="text" select="concat(',', $new.line)" />
		</x:call-template>
	</x:template>

	<x:template name="eol-if-not-last">
		<x:call-template name="if-not-last">
			<x:with-param name="text" select="$new.line" />
		</x:call-template>
	</x:template>

	<x:template name="comma-if-not-last-eol">
		<x:call-template name="comma-if-not-last" />
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="extract-table-and-view-names">
		<x:param name="text" />
		<x:param name="context" select="." />

		<x:variable name="searchable" select="concat(' ', normalize-space($text), ' ')" />

		<x:for-each select="/database/table/@name|/database/view/@name">
			<x:if test="contains($searchable, concat(' ', ., ' '))">
				<x:apply-templates select="." mode="extracted-from-text">
					<x:with-param name="context" select="$context" />
				</x:apply-templates>
			</x:if>
		</x:for-each>
	</x:template>

	<x:template match="*" mode="cascade-dependencies">
		<x:message terminate="yes">Unexpected element for cascade-dependencies: <x:value-of select="local-name()" /></x:message>
	</x:template>

	<x:template match="database/table" mode="cascade-dependencies">
		<x:for-each select="foreign-key/@table">
			<dependency><x:value-of select="." /></dependency>
		</x:for-each>
	</x:template>

	<x:template name="sql-drop-table">
		<x:param name="name" />

		<x:text>DROP TABLE IF EXISTS </x:text>
		<x:value-of select="." />
		<x:text>;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template match="entry[@type='table']" mode="drop">
		<x:call-template name="sql-drop-table">
			<x:with-param name="name" select="." />
		</x:call-template>
	</x:template>

	<x:template match="table/@name|view/@name" mode="extracted-from-text">
		<x:param name="context" />

		<dependency><x:value-of select="." /></dependency>
	</x:template>

	<x:template match="database/view" mode="cascade-dependencies">
		<x:for-each select="join/@remote|@base">
			<dependency><x:value-of select="." /></dependency>
		</x:for-each>

		<x:for-each select="sub-query">
			<x:call-template name="extract-table-and-view-names">
				<x:with-param name="text" select="sql" />
			</x:call-template>
		</x:for-each>
	</x:template>

	<x:template name="sql-drop-view">
		<x:param name="name" />

		<x:text>DROP VIEW IF EXISTS </x:text>
		<x:value-of select="." />
		<x:text>;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template match="entry[@type='view']" mode="drop">
		<x:call-template name="sql-drop-view">
			<x:with-param name="name" select="." />
		</x:call-template>
	</x:template>

	<x:template match="entry" mode="drop">
		<x:message terminate="yes">Unexpected entry in drop: <x:value-of select="." /></x:message>
	</x:template>

	<x:template name="cascade-head">
		<x:param name="index" />
		<x:param name="queue" />
		<x:param name="elements" />
		<x:param name="visited" select="''" />

		<x:choose>
			<x:when test="contains(concat(' ', $visited), concat(' ', $index, ' '))">
				<!-- already visited node, skip -->
				<x:call-template name="cascade-top">
					<x:with-param name="queue" select="$queue" />
					<x:with-param name="elements" select="$elements" />
					<x:with-param name="visited" select="$visited" />
				</x:call-template>
			</x:when>
			<x:otherwise>
				<x:variable name="head" select="$elements[number($index)]" />

				<!-- resolve current head's dependencies and filter them to not-visited ones -->
				<x:variable name="dependencies-data">
					<x:apply-templates select="$head" mode="cascade-dependencies" />
				</x:variable>

				<!-- convert dependencies-data to indices to elements -->
				<x:variable name="dependencies">
					<x:for-each select="c:node-set($dependencies-data)/dependency">
						<x:variable name="name" select="." />

						<x:for-each select="$elements">
							<x:if test="@name = $name">
								<x:value-of select="concat(position(), ' ')" />
							</x:if>
						</x:for-each>
					</x:for-each>
				</x:variable>

				<x:variable name="not-visited">
					<x:for-each select="s:split($dependencies, ' ')">
						<x:if test="not(contains(concat(' ', $visited), concat(' ', ., ' ')))">
							<!-- break self-dependency, otherwise we loop infinitely -->
							<x:if test=". != $index">
								<x:value-of select="concat(., ' ')" />
							</x:if>
						</x:if>
					</x:for-each>
				</x:variable>

				<x:if test="count(s:split(normalize-space($not-visited), ' ')) = 0">
					<!-- no dependencies, create entry -->
					<entry type="{local-name($head)}"><x:value-of select="$head/@name" /></entry>

					<!-- continue to the next element in the queue -->
					<x:call-template name="cascade-top">
						<x:with-param name="queue" select="$queue" />
						<x:with-param name="elements" select="$elements" />
						<x:with-param name="visited" select="concat($visited, $index, ' ')" />
					</x:call-template>
				</x:if>

				<!-- append to queue to delay processing -->
				<x:if test="count(s:split(normalize-space($not-visited), ' ')) &gt; 0">
					<x:call-template name="cascade-top">
						<x:with-param name="queue" select="concat($queue, $index, ' ')" />
						<x:with-param name="elements" select="$elements" />
						<x:with-param name="visited" select="$visited" />
					</x:call-template>
				</x:if>
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template name="cascade-top">
		<x:param name="elements" />
		<x:param name="visited" select="''" />
		<x:param name="queue">
			<x:for-each select="$elements">
				<x:value-of select="concat(position(), ' ')" />
			</x:for-each>
		</x:param>

		<x:if test="count(s:split(normalize-space($queue), ' ')) &gt; 0">
			<x:call-template name="cascade-head">
				<x:with-param name="index" select="number(substring-before($queue, ' '))" />
				<x:with-param name="queue" select="substring-after($queue, ' ')" />
				<x:with-param name="elements" select="$elements" />
				<x:with-param name="visited" select="$visited" />
			</x:call-template>
		</x:if>
	</x:template>

	<x:template name="drop-tables-and-views">
		<x:variable name="result-data">
			<x:call-template name="cascade-top">
				<x:with-param name="elements" select="/database/table|/database/view" />
			</x:call-template>
		</x:variable>

		<x:for-each select="c:node-set($result-data)/entry">
			<x:sort select="position()" data-type="number" order="descending" />
			<x:apply-templates select="." mode="drop" />
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
		<x:call-template name="drop-tables-and-views" />
		<x:call-template name="new-line" />
		<x:apply-templates select="table" />
		<x:apply-templates select="view" />
	</x:template>

	<x:template name="create-table">
		<x:text>CREATE TABLE </x:text>
		<x:call-template name="print-name" />
		<x:text> (</x:text>
		<x:call-template name="new-line" />
		<x:apply-templates select="column|primary-key|foreign-key|unique|check" />
		<x:text>);</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template match="database/table">
		<x:call-template name="create-table" />
		<x:call-template name="new-line" />
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

	<x:template name="create-view">
		<x:text>CREATE VIEW </x:text>
		<x:call-template name="print-name" />
		<x:text> AS</x:text>
		<x:call-template name="new-line" />
		<x:text>    SELECT</x:text>
		<x:call-template name="new-line" />

		<x:apply-templates select="map|join/map|sub-query" />

		<x:call-template name="new-line" />
		<x:text>    FROM </x:text>
		<x:call-template name="print-base" />

		<x:if test="join">
			<x:call-template name="new-line" />
			<x:apply-templates select="join" />
		</x:if>

		<x:text>;</x:text>
		<x:call-template name="new-line" />
	</x:template>

	<x:template match="database/view">
		<x:call-template name="create-view" />
		<x:call-template name="new-line" />
	</x:template>

	<x:template match="view/map">
		<x:call-template name="print-view-map">
			<x:with-param name="context" select="../@base" />
		</x:call-template>
	</x:template>

	<x:template match="view/join/map">
		<x:call-template name="print-view-map">
			<x:with-param name="context" select="../@remote" />
		</x:call-template>
	</x:template>

	<x:template match="view/sub-query">
		<x:text>        </x:text>
		<x:text>(</x:text>
		<x:value-of select="normalize-space(sql)" />
		<x:text>)</x:text>
		<x:call-template name="new-line" />
		<x:text>            </x:text>
		<x:text>AS </x:text>
		<x:value-of select="@as" />

		<x:call-template name="comma-eol-if-not-last" />
	</x:template>

	<x:template name="view-join">
		<x:param name="base" />

		<x:text>    JOIN </x:text>
		<x:call-template name="print-remote" />
		<x:text> ON </x:text>

		<x:value-of select="concat($base, '.', @on-base)" />
		<x:text> = </x:text>
		<x:value-of select="concat(@remote, '.', @on-remote)" />

		<x:call-template name="eol-if-not-last" />
	</x:template>

	<x:template match="view/join[@with-base]">
		<x:call-template name="view-join">
			<x:with-param name="base" select="@with-base" />
		</x:call-template>
	</x:template>

	<x:template match="view/join[not(@with-base)]">
		<x:call-template name="view-join">
			<x:with-param name="base" select="../@base" />
		</x:call-template>
	</x:template>

</x:stylesheet>
