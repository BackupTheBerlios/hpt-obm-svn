<?xml version="1.0" encoding="UTF-8"?>
<!--
	This stylesheet is used to transform an xmlschema to HTML.
	Using as follow:
	xsltproc database.xsl data.xml
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >
	<xsl:output method="xml" indent="yes" omit-xml-declaration="no" encoding="UTF-8"/>
	<xsl:template match="schema">
				<xsl:apply-templates select="table">
					<xsl:sort select="@name" order="ascending"/>
				</xsl:apply-templates>
	</xsl:template>

	<xsl:template match="table">
		<h2><xsl:value-of select="@name"/></h2>
		<h3>Description</h3>
		<xsl:copy-of select="descr"/> 
		<h3>Field</h3>
		<table class="field">
			<tr>
				<th>Name</th>
				<th>Type</th>
				<th>Size</th>
				<th>NULL</th>
				<th>Attributes</th>
				<th>Description</th>
			</tr>	
			<xsl:apply-templates select="field">
				<xsl:sort select="@name" order="ascending"/>
			</xsl:apply-templates>
		</table>
	</xsl:template>
	<xsl:template match="field">
		<tr>
			<td><xsl:value-of select="@name"/></td>
			<td><xsl:value-of select="@type"/></td>
			<td><xsl:value-of select="@size"/></td>
			<td><xsl:choose><xsl:when test="NOTNULL=''">N</xsl:when><xsl:otherwise>Y</xsl:otherwise></xsl:choose></td>
			<td/>
			<td><xsl:value-of select="descr"/></td>
		</tr>	
	</xsl:template>
</xsl:stylesheet>
