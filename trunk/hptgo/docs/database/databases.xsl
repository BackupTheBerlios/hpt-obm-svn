<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >
	<xsl:import href="database.xsl"/>
	<xsl:output method="html" indent="yes" omit-xml-declaration="yes" encoding="UTF-8"/>
	<xsl:template match="modules">
		<html>
			<head>
				<link rel="stylesheet" href="database.css" type="text/css"/>
			</head>
			<body>
				<h1>Table of contents</h1>
				<ul>
					<xsl:for-each select="module">
						<li><a href="#{generate-id(.)}"><xsl:value-of select="@name"/></a></li>
					</xsl:for-each>
					<li><a href="#index">Index</a></li>
				</ul>

				<h1>Description</h1>
				<xsl:apply-templates select="module"/>

				<h1 id="index">Index</h1>
					<xsl:for-each select="module">
						<xsl:for-each select="document(@href)/schema/table">
							<a href="#table_{@name}"><xsl:value-of select="@name"/></a><br/>
						</xsl:for-each>
					</xsl:for-each>

			</body>
		</html>
	</xsl:template>
	<xsl:template match="module">
		<h2 id="{generate-id(.)}">Module <xsl:value-of select="@name"/></h2>
		<center>
			<xsl:for-each select="document(@href)/schema/table">
				<xsl:sort select="@name" order="ascending"/>
				<xsl:if test="position() != 1"> - </xsl:if>
				<a href="#table_{@name}"><xsl:value-of select="@name"/></a>
			</xsl:for-each>
		</center>
		<xsl:apply-templates select="document(@href)/schema"/>
	</xsl:template>
</xsl:stylesheet>

