<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >
	<xsl:import href="database.xsl"/>
	<xsl:output method="html" indent="yes" omit-xml-declaration="yes" encoding="UTF-8"/>
	<xsl:variable name="list" select="document('modules.xml')"/>
	<xsl:template match="root">
		<html>
			<head>
				<link rel="stylesheet" href="database.css" type="text/css"/>
			</head>
			<body>
				<xsl:for-each select="$list/modules/module">
					<a href="#{position()}"><xsl:value-of select="@name"/></a><br/>
				</xsl:for-each>

				<xsl:apply-templates select="document($list/modules/module)">
					<xsl:sort select="$list/modules/module/@name" order="ascending"/>
				</xsl:apply-templates>
			</body>
		</html>
	</xsl:template>
	<xsl:template match="module">
		<a name="{position()}"/>
		<h1>Module <xsl:value-of select="@name"/></h1>
		<xsl:apply-templates select="document(@href)"/>
	</xsl:template>
</xsl:stylesheet>

