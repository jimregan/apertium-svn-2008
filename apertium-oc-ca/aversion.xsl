<?xml version="1.0" encoding="ISO-8859-1"?> <!-- -*- nxml -*- -->
<!--
 Copyright (C) 2005 Universitat d'Alacant / Universidad de Alicante

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" encoding="UTF-8"/>
<xsl:param name="aversion"/>

<xsl:template match="/">
  <xsl:value-of select="string('&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;&#xA;')"/>
  <xsl:apply-templates select="*|text()"/>
</xsl:template>

<xsl:template match="*">
  <xsl:if test="not(count(./@aversion)=1) or ./@aversion=$aversion">
    <xsl:value-of select="string('&lt;')"/>
    <xsl:value-of select="local-name(.)"/>
    <xsl:for-each select="./@*">
      <xsl:if test="not(local-name(.)=string('aversion'))">
        <xsl:value-of select="string(' ')"/>
        <xsl:value-of select="local-name(.)"/>
        <xsl:value-of select="string('=&quot;')"/>
        <xsl:value-of select="string(.)"/>
        <xsl:value-of select="string('&quot;')"/>
      </xsl:if>
    </xsl:for-each>
    <xsl:choose>
      <xsl:when test="count(text()[normalize-space(.)] | *)=0">
        <xsl:value-of select="string('/&gt;')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="string('&gt;')"/>
        <xsl:apply-templates/>
        <xsl:value-of select="string('&lt;/')"/>
        <xsl:value-of select="local-name(.)"/>
        <xsl:value-of select="string('&gt;')"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:if>
</xsl:template>

<xsl:template match="text()">
  <xsl:value-of select="."/>
</xsl:template>

</xsl:stylesheet>
