<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" indent="yes" omit-xml-declaration="no" encoding="UTF-8"/>

	<xsl:template match="/schema/table">
		<xsl:apply-templates select="field">
			<xsl:with-param name="table" select="@name"/>
		</xsl:apply-templates>
	</xsl:template>

	<xsl:template match="field">
		<xsl:param name="table"/>
		<xsl:variable name="field" select="@name"/>
		<xsl:for-each select="link">
			<xsl:variable name="table2" select="@table"/>
			<xsl:variable name="field2" select="@field"/>
			<xsl:if test="not(/schema/table[@name=$table2]/field[@name=$field2])">
				<xsl:copy-of select="."/>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

</xsl:stylesheet>


