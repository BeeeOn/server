<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:c="http://exslt.org/common"
                extension-element-prefixes="c"
		version="1.0">

	<x:import href="query.sql.xsl" />

	<x:param name="engine" select="'unknown'" />

	<x:template name="print-preamble" />

	<x:template name="generate-sql-tests">
		<c:document href="{@for-database}-{$engine}-test.sql" method="text" encoding="utf-8">
			<x:call-template name="print-preamble" />

			<x:apply-templates select="test-global/setup" />
			<x:apply-templates select="query" />
			<x:apply-templates select="test-global/teardown" />
		</c:document>
	</x:template>

	<x:template name="name-sequential-item">
		<x:variable name="num" select="position()" />

		<x:choose>
			<x:when test="@name">
				<x:value-of select="concat('#', $num, ' ', @name)" />
			</x:when>
			<x:otherwise>
				<x:value-of select="concat('#', $num)" />
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template name="remove-string-quotes">
		<x:param name="value" />
		<x:variable name="quotes">
			<x:text>&#x27;&quot;</x:text>
		</x:variable>
		<x:value-of select="translate($value, $quotes, '')" />
	</x:template>

	<x:template name="sql-print">
		<x:param name="value" />
		<x:param name="as" />

		<x:text>SELECT '</x:text>
		<x:call-template name="remove-string-quotes">
			<x:with-param name="value" select="$value" />
		</x:call-template>
		<x:text>' AS </x:text>
		<x:value-of select="$as" />
		<x:text>;&#xA;</x:text>
	</x:template>

	<x:template name="sql-print-id">
		<x:param name="as" select="concat(local-name(), '_id')" />

		<x:call-template name="sql-print">
			<x:with-param name="value" select="@id" />
			<x:with-param name="as" select="$as" />
		</x:call-template>
	</x:template>

	<x:template name="sql-print-name">
		<x:param name="as" select="concat(local-name(), '_name')" />

		<x:call-template name="sql-print">
			<x:with-param name="value" select="@name" />
			<x:with-param name="as" select="$as" />
		</x:call-template>
	</x:template>

	<x:template match="query">
		<x:text>-- Tests for </x:text>
		<x:value-of select="@id" />
		<x:text>&#xA;</x:text>

		<x:call-template name="sql-print-id" />
		<x:apply-templates select="test" />
	</x:template>

	<x:template match="test">
		<x:text>-- Test </x:text>
		<x:call-template name="name-sequential-item" />
		<x:text>&#xA;</x:text>

		<x:call-template name="sql-print-name" />

		<x:apply-templates select="setup" />
		<x:apply-templates select="check" />
		<x:apply-templates select="teardown" />
	</x:template>

	<x:template name="setup-name">
		<x:choose>
			<x:when test="local-name(..) = 'test-global'">
				<x:text>-- Global setup </x:text>
			</x:when>
			<x:otherwise>
				<x:text>-- Setup </x:text>
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template match="setup">
		<x:text>&#xA;</x:text>
		<x:call-template name="setup-name" />
		<x:call-template name="name-sequential-item" />
		<x:text>&#xA;</x:text>

		<x:apply-templates select="sql" mode="simple" />
	</x:template>

	<x:template name="teardown-name">
		<x:choose>
			<x:when test="local-name(..) = 'test-global'">
				<x:text>-- Global teardown </x:text>
			</x:when>
			<x:otherwise>
				<x:text>-- Teardown </x:text>
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template match="teardown">
		<x:text>&#xA;</x:text>
		<x:call-template name="teardown-name" />
		<x:call-template name="name-sequential-item" />
		<x:text>&#xA;</x:text>

		<x:apply-templates select="sql" mode="simple" />
	</x:template>

	<x:template match="check">
		<x:text>&#xA;</x:text>
		<x:text>-- Check </x:text>
		<x:call-template name="name-sequential-item" />
		<x:text>&#xA;</x:text>

		<x:call-template name="sql-print-name" />

		<x:apply-templates select="expect/fail-insert" mode="before-call" />
		<x:apply-templates select="expect/fail-update" mode="before-call" />
		<x:apply-templates select="expect/fail-delete" mode="before-call" />
		<x:apply-templates select="call-query" />
		<x:apply-templates select="sql" mode="simple" />
		<x:apply-templates select="expect" mode="sql" />
	</x:template>

	<x:template match="call-query">
		<x:text>-- Call query under test&#xA;</x:text>
		<x:apply-templates select="../../../define" mode="sql-direct">
			<x:with-param name="args" select="arg" />
		</x:apply-templates>
	</x:template>

	<x:template match="expect" mode="sql">
		<x:text>-- Expect</x:text>
		<x:value-of select="concat(' ', count(row))" />
		<x:text> results</x:text>
		<x:text>&#xA;</x:text>

		<x:apply-templates select="row" mode="describe-expect" />
		<x:apply-templates select="fail-insert" mode="after-call" />
		<x:apply-templates select="fail-update" mode="after-call" />
		<x:apply-templates select="fail-delete" mode="after-call" />
	</x:template>

	<x:template name="fail-query-message">
		<x:text>'</x:text>
		<x:value-of select="../../../@name" />
		<x:text> did not fail for query </x:text>
		<x:value-of select="../../../../@id" />
		<x:text> conflicting on </x:text>
		<x:value-of select="@conflict" />
		<x:text>'</x:text>
	</x:template>

	<x:template name="create-fail-trigger">
		<x:message terminate="yes">Not implemented create-fail-trigger</x:message>
	</x:template>

	<x:template name="drop-fail-trigger">
		<x:message terminate="yes">Not implemented drop-fail-trigger</x:message>
	</x:template>

	<x:template match="expect/fail-insert" mode="before-call">
		<x:text>CREATE TABLE insert_should_fail (error text);&#xA;</x:text>
		<x:call-template name="create-fail-trigger">
			<x:with-param name="target" select="'INSERT'" />
			<x:with-param name="label" select="'insert'" />
		</x:call-template>
	</x:template>

	<x:template match="expect/fail-insert" mode="after-call">
		<x:text>SELECT error from insert_should_fail;&#xA;</x:text>
		<x:call-template name="drop-fail-trigger">
			<x:with-param name="target" select="'INSERT'" />
			<x:with-param name="label" select="'insert'" />
		</x:call-template>
		<x:text>DROP TABLE insert_should_fail;&#xA;</x:text>
	</x:template>

	<x:template match="expect/fail-update" mode="before-call">
		<x:text>CREATE TABLE update_should_fail (error text);&#xA;</x:text>
		<x:call-template name="create-fail-trigger">
			<x:with-param name="target" select="'UPDATE'" />
			<x:with-param name="label" select="'update'" />
		</x:call-template>
	</x:template>

	<x:template match="expect/fail-update" mode="after-call">
		<x:text>SELECT error from update_should_fail;&#xA;</x:text>
		<x:call-template name="drop-fail-trigger">
			<x:with-param name="target" select="'UPDATE'" />
			<x:with-param name="label" select="'update'" />
		</x:call-template>
		<x:text>DROP TABLE update_should_fail;&#xA;</x:text>
	</x:template>

	<x:template match="expect/fail-delete" mode="before-call">
		<x:text>CREATE TABLE delete_should_fail (error text);&#xA;</x:text>
		<x:call-template name="create-fail-trigger">
			<x:with-param name="target" select="'DELETE'" />
			<x:with-param name="label" select="'delete'" />
		</x:call-template>
	</x:template>

	<x:template match="expect/fail-delete" mode="after-call">
		<x:text>SELECT error from delete_should_fail;&#xA;</x:text>
		<x:call-template name="drop-fail-trigger">
			<x:with-param name="target" select="'DELETE'" />
			<x:with-param name="label" select="'delete'" />
		</x:call-template>
		<x:text>DROP TABLE delete_should_fail;&#xA;</x:text>
	</x:template>

	<x:template match="row" mode="describe-expect">
		<x:variable name="i" select="position()" />

		<x:for-each select="value|null">
			<x:text>-- </x:text>
			<x:value-of select="concat('[', $i, '] ')" />
			<x:value-of select="concat(@name, ' = ')" />
			<x:choose>
				<x:when test="local-name() = 'value'">
					<x:text>'</x:text>
					<x:value-of select="." />
					<x:text>'</x:text>
				</x:when>
				<x:otherwise>
					<x:text>NULL</x:text>
				</x:otherwise>
			</x:choose>
			<x:text>&#xA;</x:text>
		</x:for-each>
	</x:template>

	<x:template name="generate-expect-values">
		<c:document href="{@for-database}-{$engine}-expect.csv" method="text" encoding="utf-8">
			<x:apply-templates select="query" mode="csv" />
		</c:document>
	</x:template>

	<x:template name="expect-header">
		<x:if test="row">
			<x:call-template name="print-csv-header">
				<x:with-param name="header">
					<x:for-each select="row[position() = 1]/value|row[position() = 1]/null">
						<x:value-of select="@name" />

						<x:if test="position() &lt; last()">
							<x:call-template name="csv-separator" />
						</x:if>
					</x:for-each>
				</x:with-param>
			</x:call-template>
		</x:if>
	</x:template>

	<x:template name="expect-footer" />

	<x:template match="query" mode="csv">
		<x:call-template name="print-csv-header">
			<x:with-param name="header" select="'query_id'" />
		</x:call-template>

		<x:call-template name="csv-quote">
			<x:with-param name="value">
				<x:call-template name="remove-string-quotes">
					<x:with-param name="value" select="@id" />
				</x:call-template>
			</x:with-param>
		</x:call-template>
		<x:text>&#xA;</x:text>

		<x:apply-templates select="test" mode="csv" />
	</x:template>

	<x:template match="test" mode="csv">
		<x:call-template name="print-csv-header">
			<x:with-param name="header" select="'test_name'" />
		</x:call-template>

		<x:call-template name="csv-quote">
			<x:with-param name="value">
				<x:call-template name="remove-string-quotes">
					<x:with-param name="value" select="@name" />
				</x:call-template>
			</x:with-param>
		</x:call-template>
		<x:text>&#xA;</x:text>

		<x:apply-templates select="check" mode="csv" />
	</x:template>

	<x:template match="check" mode="csv">
		<x:call-template name="print-csv-header">
			<x:with-param name="header" select="'check_name'" />
		</x:call-template>

		<x:call-template name="csv-quote">
			<x:with-param name="value">
				<x:call-template name="remove-string-quotes">
					<x:with-param name="value" select="@name" />
				</x:call-template>
			</x:with-param>
		</x:call-template>
		<x:text>&#xA;</x:text>

		<x:apply-templates select="expect" mode="csv" />
	</x:template>

	<x:template match="expect" mode="csv">
		<x:call-template name="expect-header" />

		<x:if test="row">
			<x:for-each select="row">
				<x:for-each select="value|null">
					<x:apply-templates select="." mode="csv" />

					<x:if test="position() &lt; last()">
						<x:call-template name="csv-separator" />
					</x:if>
				</x:for-each>

				<x:call-template name="csv-new-line" />
			</x:for-each>
		</x:if>

		<x:call-template name="expect-footer" />
	</x:template>

	<x:template name="print-csv-header">
		<x:param name="header" select="''" />

		<x:value-of select="$header" />
		<x:call-template name="csv-new-line" />
	</x:template>

	<x:template name="csv-new-line">
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template name="csv-separator">
		<x:text>,</x:text>
	</x:template>

	<x:template name="csv-quote">
		<x:param name="value" select="." />

		<x:choose>
			<x:when test="contains($value, ' ')">
				<x:text>"</x:text>
				<x:value-of select="$value" />
				<x:text>"</x:text>
			</x:when>
			<x:when test="string-length($value) = 0">
				<x:text>""</x:text>
			</x:when>
			<x:otherwise>
				<x:value-of select="$value" />
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template match="value" mode="csv">
		<x:call-template name="csv-quote" />
	</x:template>

	<x:template match="null" mode="csv" />

</x:stylesheet>
