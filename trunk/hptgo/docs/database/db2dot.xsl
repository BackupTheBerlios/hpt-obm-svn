<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  xmlns:dotml="http://www.martin-loetzsch.de/DOTML">
	<xsl:output method="xml" indent="yes" omit-xml-declaration="no" encoding="UTF-8"/>

	<xsl:variable name="all">0</xsl:variable>
	
	<xsl:template match="/schema">
		<dotml:graph rankdir="LR" file-name="test">
			<xsl:apply-templates select="table" mode="node"/>
			<xsl:apply-templates select="table" mode="edge"/>
		</dotml:graph>
	</xsl:template>

	<xsl:template match="table" mode="edge">
		<!-- xsl:variable name="name" select="@name"/ -->
		<!-- Only process node if there is a link to this node -->
		<!--
		<xsl:if test="/schema/table/field/link[@table=$name]">
			<xsl:if test="$all != '0' or /schema/table/field[link/@table=$name]/KEY or /schema/table/field[link/@table=$name]/PRIMARY">
		-->
				<xsl:apply-templates select="field" mode="edge">
					<xsl:with-param name="table" select="@name"/>
				</xsl:apply-templates>
		<!--
			</xsl:if>
		</xsl:if>
		-->
	</xsl:template>

	<xsl:template match="field" mode="edge">
		<!--
			Source:
			$table = table name
			$field = field name
		-->
		<xsl:param name="table"/>
		<xsl:variable name="field" select="@name"/>

		<!-- If all is 0, only show links from KEY/PRIMARY key -->
		<xsl:if test="$all = '1' or PRIMARY">
			<!-- Process link nodes -->
			<xsl:for-each select="link">
				<!--
					Target:
					$table2 = table name
					$field2 = field name
				-->
				<xsl:variable name="table2" select="@table"/>
				<xsl:variable name="field2" select="@field"/>

				<!-- If target exists -->
				<xsl:if test="/schema/table[@name=$table2]/field[@name=$field2]">
					
						<xsl:comment>
							<xsl:value-of select="$table"/>:<xsl:value-of select="$field"/>
							<xsl:text> -&gt; </xsl:text>
							<xsl:value-of select="$table2"/>:<xsl:value-of select="$field2"/>
						</xsl:comment>
					<dotml:edge>

						<xsl:attribute name="from">
							<xsl:value-of select="generate-id(/schema/table[@name=$table]/field[@name=$field])"/>
							<!-- xsl:value-of select="$table"/>_<xsl:value-of select="$field"/-->
						</xsl:attribute>

						<xsl:attribute name="to">
							<xsl:value-of select="generate-id(/schema/table[@name=$table2]/field[@name=$field2])"/>
							<!--xsl:value-of select="$table2"/>_<xsl:value-of select="$field2"/-->
						</xsl:attribute>

					</dotml:edge>

				</xsl:if>
				<!-- End if target exists -->

			</xsl:for-each>
			<!-- End for-each select="link" -->
		</xsl:if>
	</xsl:template>

	<!-- Produce dotml:record -->
	<xsl:template name="record">
		<xsl:param name="table"/>
		<dotml:record>
			<dotml:node id="head" label="{$table}"/>
			<xsl:for-each select="field">
				<!-- Only show fields that have links or are linked -->
				<xsl:variable name="field" select="@name"/>
				<xsl:if test="link or /schema/table/field/link[@table=$table and @field=$field]">
					<dotml:node id="{generate-id(.)}" label="{@name}"/>
					<!--dotml:node id="{$name}_{@name}" label="{@name}"/-->
				</xsl:if>
			</xsl:for-each>
		</dotml:record>
	</xsl:template>

	<xsl:template match="table" mode="node">
		<xsl:variable name="name" select="@name"/>

		<xsl:choose>
			<xsl:when test="$all = '1'">
				<xsl:if test="field/link or /schema/table/field/link[@table=$name]">
					<xsl:call-template name="record">
						<xsl:with-param name="table" select="$name"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:when>

			<xsl:otherwise>
				<xsl:if test="field[link]/PRIMARY or /schema/table/field[link/@table=$name]/PRIMARY">
					<xsl:call-template name="record">
						<xsl:with-param name="table" select="$name"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>

