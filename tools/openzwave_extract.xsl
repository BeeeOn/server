<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		xmlns:e="http://exslt.org/common"
		xmlns:zwave="http://code.google.com/p/open-zwave/"
		extension-element-prefixes="e zwave"
		version="1.0">

	<x:output method="text" encoding="utf-8" />
	<x:strip-space elements="*" />

	<!-- generate enums_zwave.xml and devices_zwave.xml -->
	<x:template match="/">
		<e:document href="enums_zwave.xml"
				indent="yes"
				method="xml"
				encoding="utf-8">
			<x:apply-templates mode="enums" />
		</e:document>

		<e:document href="devices_zwave.xml"
				indent="yes"
				method="xml"
				encoding="utf-8">
			<x:apply-templates mode="devices" />
		</e:document>
	</x:template>

	<!-- enums.xml -->

	<x:template match="zwave:ManufacturerSpecificData" mode="enums">
		<enums>
			<x:apply-templates mode="enums" />
		</enums>
	</x:template>

	<x:template match="zwave:Manufacturer" mode="enums">
		<x:comment>
			<x:text> Manufacturer: </x:text>
			<x:value-of select="concat(@name, '(', @id, ') ')" />
		</x:comment>
		<x:apply-templates mode="enums" />
	</x:template>

	<x:template match="zwave:Manufacturer/zwave:Product" mode="enums">
		<x:variable name="product" select="document(@config)/zwave:Product" />

		<x:comment>
			<x:text> Product: </x:text>
			<x:value-of select="concat(@name, ' (', @type, ':', @id, ') ')" />
		</x:comment>

		<x:comment>
			<x:text> File: </x:text>
			<x:value-of select="concat(@config, ' ')" />
		</x:comment>

		<x:apply-templates select="$product" mode="enums">
			<x:with-param name="id" select="concat(@type, '_', @id)" />
		</x:apply-templates>
	</x:template>

	<x:template match="zwave:Product" mode="enums">
		<x:param name="id" />

		<x:apply-templates select="zwave:CommandClass/zwave:Value[zwave:Item]" mode="enums">
			<x:with-param name="id" select="$id" />
		</x:apply-templates>
	</x:template>

	<x:template match="zwave:Product/zwave:CommandClass/zwave:Value[zwave:Item]" mode="enums">
		<x:param name="id" />

		<x:comment>
			<x:text> Enum: </x:text>
			<x:value-of select="concat(../@id, ':', @index, ' ')" />
		</x:comment>
		<enum id="zwave_{$id}_{../@id}_{@index}">
			<values>
			<x:for-each select="zwave:Item">
				<value equals="{@value}">
					<x:value-of select="@label" />
				</value>
			</x:for-each>
			</values>
		</enum>
	</x:template>

	<!-- devices.xml -->

	<x:template match="zwave:ManufacturerSpecificData" mode="devices">
		<devices>
			<x:apply-templates select="zwave:Manufacturer" mode="devices" />
		</devices>
	</x:template>

	<x:template match="zwave:Manufacturer" mode="devices">
		<x:for-each select="zwave:Product">
			<x:variable name="product" select="document(@config)/zwave:Product" />

			<x:comment>
				<x:text> File: </x:text>
				<x:value-of select="concat(@config, ' ')" />
			</x:comment>

			<device id="0x1{../@id}{@id}">
				<name><x:value-of select="@name" /></name>
				<vendor><x:value-of select="../@name" /></vendor>
				<match>
					<exact name="{@id}" vendor="{../@id}" />
					<exact name="{@name}" vendor="{../@name}" />
				</match>

				<modules>
					<x:comment> TODO </x:comment>
					<x:comment>
						<x:apply-templates
							select="$product/zwave:CommandClass/zwave:Value[zwave:Item]"
							mode="devices" />
					</x:comment>
				</modules>
			</device>
		</x:for-each>
	</x:template>

	<x:template match="zwave:Product/zwave:CommandClass/zwave:Value[zwave:Item]" mode="devices">
		<x:text>&#xA;        </x:text>
		<x:text>id: "</x:text>
		<x:value-of select="../@id" />
		<x:text>", label: "</x:text>
		<x:value-of select="@label" />
		<x:text>", type: :</x:text>
		<x:value-of select="@type" />
		<x:text>", index: :</x:text>
		<x:value-of select="@index" />
		<x:text>", units: "</x:text>
		<x:value-of select="@units" />
		<x:text>"</x:text>

		<x:if test="position() = last()">
			<x:text>&#xA;      </x:text>
		</x:if>
	</x:template>

</x:stylesheet>
