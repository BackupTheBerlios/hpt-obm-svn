<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >
	<xsl:output method="text" indent="yes" omit-xml-declaration="yes" encoding="UTF-8"/>

	<xsl:variable name="all" select="0"/>
	
	<xsl:template match="/">
		<xsl:text>digraph schema { rankdir="LR";</xsl:text>
		<xsl:apply-templates mode="node"/>
		<xsl:apply-templates mode="edge"/>
		<xsl:text>}</xsl:text>
	</xsl:template>

	<xsl:template match="table" mode="edge">
		<xsl:variable name="name" select="@name"/>
		<!-- Only process node if there is a link to this node -->
		<xsl:if test="/schema/table/field/link[@table=$name]">
			<xsl:apply-templates select="field" mode="edge">
				<xsl:with-param name="table" select="@name"/>
			</xsl:apply-templates>
		</xsl:if>
	</xsl:template>

	<xsl:template match="field" mode="edge">
		<xsl:param name="table"/>
		<xsl:variable name="field" select="@name"/>
		<xsl:if test="$all != '0' or KEY|PRIMARY">
			<xsl:for-each select="link">
				<xsl:variable name="table2" select="@table"/>
				<xsl:variable name="field2" select="@field"/>
				<xsl:if test="/schema/table[@name=$table2]/field[@name=$field2]">
					<xsl:value-of select="generate-id(/schema/table[@name=$table])"/>:<xsl:value-of select="generate-id(/schema/table[@name=$table]/field[@name=$field])"/> -&gt; <xsl:value-of select="generate-id(/schema/table[@name=$table2])"/>:<xsl:value-of select="generate-id(/schema/table[@name=$table2]/field[@name=$field2])"/><xsl:text>;</xsl:text>
				</xsl:if>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>

	<xsl:template match="table" mode="node">
		<xsl:variable name="name" select="@name"/>
		<!-- Only process node if there is a link to this node -->
		<xsl:if test="/schema/table/field/link[@table=$name]">
			<xsl:value-of select="generate-id(.)"/>
			<xsl:text>[shape="record",label="&lt;head&gt; </xsl:text>
			<xsl:value-of select="@name"/><xsl:text>|&lt;a&gt; </xsl:text>
			<xsl:for-each select="field">
				<xsl:variable name="field" select="@name"/>
				<!-- Only show fields that have links or are linked -->
				<xsl:if test="/schema/table/field/link[@table=$name and @field=$field] or link"> |&lt;<xsl:value-of select="generate-id(.)"/>&gt; <xsl:value-of select="@name"/>
				</xsl:if>
			</xsl:for-each>
			<xsl:text>"];</xsl:text>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>

