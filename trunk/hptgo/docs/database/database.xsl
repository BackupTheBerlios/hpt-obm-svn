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
		<h2 id="{concat('table_',@name)}">Table <xsl:value-of select="@name"/></h2>
		<xsl:if test="DROP">DROM</xsl:if>
		<h3>Description</h3>
		<xsl:copy-of select="descr"/> 
		<xsl:apply-templates select="constraint"/>
		<h3>Field</h3>
		<table class="field">
			<tr>
				<th nowrap="1"></th>
				<th nowrap="1">Name</th>
				<th nowrap="1">Type</th>
				<th nowrap="1">Attributes</th>
				<th nowrap="1">Value</th>
				<th nowrap="1">Constraints</th>
				<th nowrap="1" width="100%">Description</th>
				<th nowrap="1">Link</th>
			</tr>	
			<xsl:apply-templates select="field">
				<xsl:with-param name="table_name" select="@name"/>
				<!-- xsl:sort select="@name" order="ascending"/ -->
			</xsl:apply-templates>
		</table>
	</xsl:template>
	<xsl:template match="field">
		<xsl:param name="table_name"/>
		<tr id="field_{$table_name}_{@name}" class="row_{position() mod 2}">
			<td nowrap="1">
				<xsl:if test="PRIMARY">P</xsl:if>
				<xsl:if test="KEY">K</xsl:if>
			</td>
			<td nowrap="1"><!-- Name -->
				<xsl:choose>
					<xsl:when test="PRIMARY">
						<b>
							<xsl:value-of select="@name"/>
						</b>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@name"/>
					</xsl:otherwise>
				</xsl:choose>
			</td>
			<td nowrap="1"><!-- Type -->
				<xsl:choose>
					<xsl:when test="@type = 'I'">Integer</xsl:when>
					<xsl:when test="@type = 'C'">Varchar</xsl:when>
					<xsl:when test="@type = 'X'">Text</xsl:when>
					<xsl:when test="@type = 'B'">BLOB</xsl:when>
					<xsl:when test="@type = 'XL'">CLOB</xsl:when>
					<xsl:when test="@type = 'L'">Boolean</xsl:when>
					<xsl:when test="@type = 'D'">Date</xsl:when>
					<xsl:when test="@type = 'T'">Timestamp</xsl:when>
					<xsl:when test="@type = 'F'">Float</xsl:when>
					<xsl:otherwise>Unknown<xsl:value-of select="@type"/></xsl:otherwise>
				</xsl:choose>
				<xsl:if test="@size != ''">
					(<xsl:value-of select="@size"/>)
				</xsl:if>
			</td>
			<td><!-- Attributes -->
				<xsl:choose>
					<xsl:when test="NOTNULL"></xsl:when>
					<xsl:otherwise>NULL</xsl:otherwise>
				</xsl:choose>
				<xsl:if test="AUTO"> AUTO</xsl:if>
				<xsl:if test="AUTOINCREMENT"> AUTOINCREMENT</xsl:if>
				<xsl:if test="NOQUOTE"> NOQUOTE</xsl:if>
			</td>
			<td><!-- Default value -->
				<xsl:value-of select="DEFAULT/@value"/>
			</td>
			<td><!-- Constraints -->
				<xsl:copy-of select="constraint"/>
			</td>
			<td><!-- Description -->
				<xsl:value-of select="descr"/>
			</td>
			<td><!-- Link -->
				<xsl:apply-templates select="link"/>
			</td>
		</tr>	
	</xsl:template>

	<xsl:template match="link">
		<a href="#table_{@table}"><xsl:value-of select="@table"/></a>.<a href="#field_{@table}_{@field}"><xsl:value-of select="@field"/></a>
	</xsl:template>
</xsl:stylesheet>
