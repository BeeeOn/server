<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:import href="query.test.postgre.xsl" />
	<x:import href="util.isql.xsl" />

	<x:param name="engine" select="'postgre-isql'" />

	<x:output method="text" />

	<x:template name="print-preamble" />

	<x:template name="create-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:call-template name="create-psql-fail-trigger-function">
					<x:with-param name="label" select="$label" />
				</x:call-template>
			</x:with-param>
		</x:call-template>
		<x:call-template name="new-line" />

		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:call-template name="create-psql-fail-trigger">
					<x:with-param name="target" select="$target" />
					<x:with-param name="label" select="$label" />
				</x:call-template>
			</x:with-param>
		</x:call-template>
		<x:call-template name="new-line" />
	</x:template>

	<x:template name="drop-fail-trigger">
		<x:param name="target" />
		<x:param name="label" />

		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:call-template name="drop-psql-fail-trigger">
					<x:with-param name="label" select="$label" />
				</x:call-template>
			</x:with-param>
		</x:call-template>
		<x:call-template name="new-line" />

		<x:call-template name="remove-cr-lf">
			<x:with-param name="text">
				<x:call-template name="drop-psql-fail-trigger-function">
					<x:with-param name="label" select="$label" />
				</x:call-template>
			</x:with-param>
		</x:call-template>
		<x:call-template name="new-line" />
	</x:template>

	<x:template match="value" mode="csv">
		<x:variable name="value">
			<x:choose>
				<x:when test="@type = 'uuid'">
					<x:call-template name="to-upper" />
				</x:when>
				<x:otherwise>
					<x:value-of select="." />
				</x:otherwise>
			</x:choose>
		</x:variable>

		<x:call-template name="csv-quote">
			<x:with-param name="value" select="$value" />
		</x:call-template>
	</x:template>

	<x:template match="true" mode="csv">
		<x:text>1</x:text>
	</x:template>

	<x:template match="false" mode="csv">
		<x:text>0</x:text>
	</x:template>

</x:stylesheet>
