<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >
	<xsl:output method="xml" indent="yes" omit-xml-declaration="no" encoding="UTF-8"/>
	<xsl:template match="/modules">
		<xsl:processing-instruction name="xml-stylesheet">type="text/xsl" href="databases2.xsl"</xsl:processing-instruction>
		<xsl:copy>
			<xsl:copy-of select="@*"/>
			<xsl:apply-templates/>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="module">
		<xsl:copy>
			<xsl:copy-of select="@*"/>
			<xsl:copy-of select="document(@href)/schema"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
