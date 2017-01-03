<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:c="http://exslt.org/common"
                extension-element-prefixes="c"
		version="1.0">

	<x:output method="text" />

	<x:param name="engine" select="'sqlite'" />

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

	<x:template match="/">
		<x:apply-templates select="query-set" />
	</x:template>

	<x:template name="test-preamble">
		<x:if test="$engine = 'sqlite'">
			<x:text>.mode csv&#xA;</x:text>
			<x:text>.headers on&#xA;</x:text>
			<x:text>.output </x:text>
			<x:value-of select="concat(@for-database, '-', $engine, '-test.csv', '&#xA;')" />
			<x:text>PRAGMA foreign_keys = ON;&#xA;</x:text>
		</x:if>
		<x:if test="$engine = 'postgre'">
			<x:text>\f ','&#xA;</x:text>
			<x:text>\a&#xA;</x:text>
			<x:text>\t on&#xA;</x:text>
			<x:text>\o '</x:text>
			<x:value-of select="concat(@for-database, '-', $engine, '-test.csv')" />
			<x:text>'&#xA;</x:text>
		</x:if>
	</x:template>

	<x:template match="query-set">
		<c:document href="{@for-database}-{$engine}-test.sql" method="text" encoding="utf-8">
			<x:call-template name="test-preamble" />

			<x:apply-templates select="test-global/setup" />
			<x:apply-templates select="query" />
			<x:apply-templates select="test-global/teardown" />
		</c:document>

		<c:document href="{@for-database}-{$engine}-expect.csv" method="text" encoding="utf-8">
			<x:apply-templates select="query/test/check/expect" mode="csv" />
		</c:document>
	</x:template>

	<x:template match="query">
		<x:text>-- Tests for </x:text>
		<x:value-of select="@id" />
		<x:text>&#xA;</x:text>

		<x:apply-templates select="test" />
	</x:template>

	<x:template match="test">
		<x:text>-- Test </x:text>
		<x:call-template name="name-sequential-item" />
		<x:text>&#xA;</x:text>

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

	<x:template match="check">
		<x:text>&#xA;</x:text>
		<x:text>-- Check </x:text>
		<x:call-template name="name-sequential-item" />
		<x:text>&#xA;</x:text>

		<x:apply-templates select="expect/fail-insert" mode="before-call" />
		<x:apply-templates select="expect/fail-update" mode="before-call" />
		<x:apply-templates select="expect/fail-delete" mode="before-call" />
		<x:apply-templates select="call-query" />
		<x:apply-templates select="sql" mode="simple" />
		<x:apply-templates select="expect" />
	</x:template>

	<x:template match="call-query">
		<x:text>-- Call query under test&#xA;</x:text>
		<x:apply-templates select="../../../define" mode="check">
			<x:with-param name="args" select="arg" />
		</x:apply-templates>
	</x:template>

	<x:template match="define" mode="check">
		<x:param name="args" />

		<x:if test="not(@engine) or $engine = @engine">
			<x:apply-templates select="text()|value" mode="sql-direct">
				<x:with-param name="engine" select="$engine" />
				<x:with-param name="args" select="$args" />
			</x:apply-templates>

			<x:text>;&#xA;</x:text>
		</x:if>
	</x:template>

	<x:template match="define/text()" mode="sql-direct">
		<x:param name="args" />
		<x:value-of select="normalize-space(.)" />
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

	<x:template match="expect/fail-insert" mode="before-call">
		<x:text>-- Prepare for checking failing insert&#xA;</x:text>
		<x:if test="$engine = 'sqlite'">
			<x:text>CREATE TABLE insert_should_fail (error text);&#xA;</x:text>
			<x:text>CREATE TRIGGER trigger_after_insert AFTER INSERT ON </x:text>
			<x:value-of select="@table" />
			<x:text> BEGIN INSERT INTO insert_should_fail (error) VALUES ('</x:text>
			<x:value-of select="concat(../../../@name, ' did not fail for query ', ../../../../@id, ' conflicting on ', @conflict)" />
			<x:text>'); END;&#xA;</x:text>
		</x:if>
		<x:if test="$engine = 'postgre'">
			<x:text>CREATE TABLE insert_should_fail (error text);&#xA;</x:text>
			<x:text>CREATE OR REPLACE FUNCTION fail_after_insert() RETURNS trigger AS $xxx$&#xA;</x:text>
			<x:text>  BEGIN INSERT INTO insert_should_fail (error) VALUES ('</x:text>
			<x:value-of select="concat(../../../@name, ' did not fail for query ', ../../../../@id, ' conflicting on ', @conflict)" />
			<x:text>'); RETURN NULL; END; $xxx$ LANGUAGE plpgsql;&#xA;</x:text>
			<x:text>CREATE TRIGGER trigger_after_insert AFTER INSERT ON </x:text>
			<x:value-of select="@table" />
			<x:text> EXECUTE PROCEDURE fail_after_insert();&#xA;</x:text>
		</x:if>
	</x:template>

	<x:template match="expect/fail-insert" mode="after-call">
		<x:text>-- Test insert has failed (no output)&#xA;</x:text>
		<x:text>SELECT error from insert_should_fail;&#xA;</x:text>
		<x:if test="$engine = 'sqlite'">
			<x:text>DROP TRIGGER trigger_after_insert;&#xA;</x:text>
		</x:if>
		<x:if test="$engine = 'postgre'">
			<x:text>DROP TRIGGER trigger_after_insert ON </x:text>
			<x:value-of select="@table" />
			<x:text>;&#xA;</x:text>
			<x:text>DROP FUNCTION fail_after_insert();&#xA;</x:text>
		</x:if>
		<x:text>DROP TABLE insert_should_fail;&#xA;</x:text>
	</x:template>

	<x:template match="expect/fail-update" mode="before-call">
		<x:text>-- Prepare for checking failing update&#xA;</x:text>
		<x:if test="$engine = 'sqlite'">
			<x:text>CREATE TABLE update_should_fail (error text);&#xA;</x:text>
			<x:text>CREATE TRIGGER trigger_after_update AFTER UPDATE ON </x:text>
			<x:value-of select="@table" />
			<x:text> BEGIN INSERT INTO update_should_fail (error) VALUES ('</x:text>
			<x:value-of select="concat(../../../@name, ' did not fail for query ', ../../../../@id, ' conflicting on ', @conflict)" />
			<x:text>'); END;&#xA;</x:text>
		</x:if>
		<x:if test="$engine = 'postgre'">
			<x:text>CREATE TABLE update_should_fail (error text);&#xA;</x:text>
			<x:text>CREATE OR REPLACE FUNCTION fail_after_update() RETURNS trigger AS $xxx$&#xA;</x:text>
			<x:text>  BEGIN INSERT INTO update_should_fail (error) VALUES ('</x:text>
			<x:value-of select="concat(../../../@name, ' did not fail for query ', ../../../../@id, ' conflicting on ', @conflict)" />
			<x:text>'); RETURN NULL; END; $xxx$ LANGUAGE plpgsql;&#xA;</x:text>
			<x:text>CREATE TRIGGER trigger_after_update AFTER UPDATE ON </x:text>
			<x:value-of select="@table" />
			<x:text> EXECUTE PROCEDURE fail_after_update();&#xA;</x:text>
		</x:if>
	</x:template>

	<x:template match="expect/fail-update" mode="after-call">
		<x:text>-- Test update has failed (no output)&#xA;</x:text>
		<x:text>SELECT error from update_should_fail;&#xA;</x:text>
		<x:if test="$engine = 'sqlite'">
			<x:text>DROP TRIGGER trigger_after_update;&#xA;</x:text>
		</x:if>
		<x:if test="$engine = 'postgre'">
			<x:text>DROP TRIGGER trigger_after_update ON </x:text>
			<x:value-of select="@table" />
			<x:text>;&#xA;</x:text>
			<x:text>DROP FUNCTION fail_after_update();&#xA;</x:text>
		</x:if>
		<x:text>DROP TABLE update_should_fail;&#xA;</x:text>
	</x:template>

	<x:template match="expect/fail-delete" mode="before-call">
		<x:text>-- Prepare for checking failing delete&#xA;</x:text>
		<x:if test="$engine = 'sqlite'">
			<x:text>CREATE TABLE delete_should_fail (error text);&#xA;</x:text>
			<x:text>CREATE TRIGGER trigger_after_delete AFTER DELETE ON </x:text>
			<x:value-of select="@table" />
			<x:text> BEGIN INSERT INTO delete_should_fail (error) VALUES ('</x:text>
			<x:value-of select="concat(../../../@name, ' did not fail for query ', ../../../../@id, ' conflicting on ', @conflict)" />
			<x:text>'); END;&#xA;</x:text>
		</x:if>
		<x:if test="$engine = 'postgre'">
			<x:text>CREATE TABLE delete_should_fail (error text);&#xA;</x:text>
			<x:text>CREATE OR REPLACE FUNCTION fail_after_delete() RETURNS trigger AS $xxx$&#xA;</x:text>
			<x:text>  BEGIN INSERT INTO delete_should_fail (error) VALUES ('</x:text>
			<x:value-of select="concat(../../../@name, ' did not fail for query ', ../../../../@id, ' conflicting on ', @conflict)" />
			<x:text>'); RETURN NULL; END; $xxx$ LANGUAGE plpgsql;&#xA;</x:text>
			<x:text>CREATE TRIGGER trigger_after_delete AFTER DELETE ON </x:text>
			<x:value-of select="@table" />
			<x:text> EXECUTE PROCEDURE fail_after_delete();&#xA;</x:text>
		</x:if>
	</x:template>

	<x:template match="expect/fail-delete" mode="after-call">
		<x:text>-- Test delete has failed (no output)&#xA;</x:text>
		<x:text>SELECT error from delete_should_fail;&#xA;</x:text>
		<x:if test="$engine = 'sqlite'">
			<x:text>DROP TRIGGER trigger_after_delete;&#xA;</x:text>
		</x:if>
		<x:if test="$engine = 'postgre'">
			<x:text>DROP TRIGGER trigger_after_delete ON </x:text>
			<x:value-of select="@table" />
			<x:text>;&#xA;</x:text>
			<x:text>DROP FUNCTION fail_after_delete();&#xA;</x:text>
		</x:if>
		<x:text>DROP TABLE delete_should_fail;&#xA;</x:text>
	</x:template>

	<x:template match="expect">
		<x:text>-- Expect</x:text>
		<x:value-of select="concat(' ', count(row))" />
		<x:text> results</x:text>
		<x:text>&#xA;</x:text>

		<x:apply-templates select="row" mode="describe-expect" />
		<x:apply-templates select="fail-insert" mode="after-call" />
		<x:apply-templates select="fail-update" mode="after-call" />
		<x:apply-templates select="fail-delete" mode="after-call" />
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

	<x:template name="csv-new-line">
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template name="csv-separator">
		<x:text>,</x:text>
	</x:template>

	<x:template name="csv-quote-value">
		<x:param name="value" select="." />
		<x:variable name="is.null" select="local-name() = 'null'" />

		<x:choose>
			<x:when test="$engine = 'postgre'">
				<x:if test="not($is.null)">
					<x:value-of select="$value" />
				</x:if>
			</x:when>
			<x:when test="contains($value, ' ')">
				<x:text>"</x:text>
				<x:value-of select="$value" />
				<x:text>"</x:text>
			</x:when>
			<x:otherwise>
				<x:if test="not($is.null)">
					<x:value-of select="$value" />
				</x:if>
			</x:otherwise>
		</x:choose>
	</x:template>

	<x:template name="print-csv-header">
		<x:for-each select="row[position() = 1]/value|row[position() = 1]/null">
			<x:value-of select="@name" />

			<x:if test="position() &lt; last()">
				<x:call-template name="csv-separator" />
			</x:if>
		</x:for-each>

		<x:call-template name="csv-new-line" />
	</x:template>

	<x:template name="expect-header">
		<x:if test="row and $engine = 'sqlite'">
			<x:call-template name="print-csv-header" />
		</x:if>
	</x:template>

	<x:template name="expect-footer">
	</x:template>

	<x:template match="expect" mode="csv">
		<x:call-template name="expect-header" />

		<x:if test="row">
			<x:for-each select="row">
				<x:for-each select="value|null">
					<x:call-template name="csv-quote-value" />

					<x:if test="position() &lt; last()">
						<x:call-template name="csv-separator" />
					</x:if>
				</x:for-each>

				<x:call-template name="csv-new-line" />
			</x:for-each>
		</x:if>

		<x:call-template name="expect-footer" />
	</x:template>

</x:stylesheet>
