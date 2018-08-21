<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:e="http://exslt.org/common"
		xmlns:zwave="http://code.google.com/p/open-zwave/"
		extension-element-prefixes="e zwave"
		version="1.0">

	<x:output method="text" encoding="utf-8" />
	<x:strip-space elements="*" />

	<x:key name="devices" match="/devices/device" use="vendor" />
	<x:variable name="types.xml" select="document('types.xml', .)/types" />

	<!-- generate devices.md -->
	<x:template match="/">
		<x:apply-templates mode="markdown" />
	</x:template>

	<x:template match="devices" mode="markdown">
		<x:text># Supported devices&#xA;</x:text>
		<x:text>&#xA;</x:text>

		<x:for-each select="device[generate-id(.) = generate-id(key('devices', ./vendor)[1])]">
			<x:sort select="vendor" />
			<x:text>* </x:text>
			<x:value-of select="vendor" />
			<x:text>&#xA;</x:text>

			<x:for-each select="key('devices', vendor)">
				<x:text>  * [</x:text>
				<x:value-of select="name" />
				<x:text>](#</x:text>
				<x:value-of select="@id" />
				<x:text>)&#xA;</x:text>
			</x:for-each>
		</x:for-each>

		<x:text>&#xA;</x:text>

		<x:apply-templates mode="markdown" />
	</x:template>

	<x:template match="device" mode="markdown">
		<x:text>## </x:text>
		<x:text disable-output-escaping="yes"> &lt;a name="</x:text>
		<x:value-of select="@id" />
		<x:text disable-output-escaping="yes">"&gt;&lt;/a&gt;</x:text>

		<x:value-of select="name" />
		<x:text>&#xA;&#xA;</x:text>

		<x:text>Vendor: </x:text>
		<x:value-of select="vendor" />
		<x:text>&#xA;&#xA;</x:text>

		<x:text>Identified as:&#xA;&#xA;</x:text>
		<x:apply-templates select="match" mode="markdown" />
		<x:text>&#xA;</x:text>

		<x:text>Modules:&#xA;&#xA;</x:text>
		<x:apply-templates select="modules" mode="markdown" />
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template match="match" mode="markdown">
		<x:apply-templates mode="markdown" />
	</x:template>

	<x:template match="exact" mode="markdown">
		<x:text>* </x:text>
		<x:text>name: *</x:text>
		<x:value-of select="@name" />
		<x:text>*, vendor: *</x:text>
		<x:value-of select="@vendor" />
		<x:text>*</x:text>
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template match="pattern" mode="markdown">
		<x:text>* </x:text>
		<x:text>name: `</x:text>
		<x:value-of select="@name" />
		<x:text>`, vendor: `</x:text>
		<x:value-of select="@vendor" />
		<x:text>`</x:text>
		<x:text>&#xA;</x:text>
	</x:template>

	<x:template match="modules" mode="markdown">
		<x:apply-templates mode="markdown" />
	</x:template>

	<x:template match="sensor|control" mode="markdown">
		<x:text>* </x:text>
		<x:value-of select="name" />

		<x:text> (</x:text>
		<x:value-of select="local-name()" />
		<x:text> </x:text>
		<x:value-of select="@id" />
		<x:text>, </x:text>
		<x:value-of select="@type" />
		<x:text>)</x:text>

		<x:text>&#xA;</x:text>
	</x:template>

</x:stylesheet>
