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

<xsl:template name="processTags">
  <xsl:param name="tags"/>
  <xsl:if test="not($tags=string(''))">
    <xsl:choose>
      <xsl:when test="not(contains($tags, string('.')))">
	<xsl:choose>
	  <xsl:when test="$tags=string('*')">
	    <xsl:value-of select="string(' __TAGS__ ')"/>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="string('&lt;')"/>
	    <xsl:value-of select="$tags"/>
	    <xsl:value-of select="string('&gt;')"/>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:when>
      <xsl:when test="substring-before($tags, string('.'))=string('*')">
	<xsl:value-of select="string('{tags}*')"/>
	<xsl:call-template name="processTags">
	  <xsl:with-param name="tags" 
			  select="substring-after($tags, string('.'))"/>
	</xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
	<xsl:value-of select="string('&lt;')"/>
	<xsl:value-of select="substring-before($tags, string('.'))"/>
	<xsl:value-of select="string('&gt;')"/>
	<xsl:call-template name="processTags">
	  <xsl:with-param name="tags" 
			  select="substring-after($tags, string('.'))"/>
	</xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:if>
</xsl:template>

<xsl:template match="transfer">
  <xsl:for-each select="/transfer/section-def-cats/def-cat">
    <xsl:value-of select="string('REGEX ')"/>
    <xsl:value-of select="string('\^(')"/>
    <xsl:for-each select="./cat-item">
      <xsl:choose>
	<xsl:when test="count(./@lemma)=1">
	  <xsl:value-of select="./@lemma"/>
	</xsl:when>
	<xsl:otherwise>
          <xsl:value-of select="string(' __LEMMA__ ')"/>
	</xsl:otherwise>
      </xsl:choose>
      <xsl:if test="count(./@tags)=1">
	<xsl:call-template name="processTags">
	  <xsl:with-param name="tags" select="./@tags"/>
	</xsl:call-template>
      </xsl:if>
      <xsl:if test="not(position()=last())">
	<xsl:value-of select="string('|')"/>
      </xsl:if>
    </xsl:for-each>
    <xsl:value-of select="string(')\$&#xA;')"/>  
  </xsl:for-each>
<xsl:for-each select="./section-rules/rule[count(./pattern/pattern-item)&gt;1]">
  <xsl:value-of select="string('RULE ')"/>
  <xsl:for-each select="./pattern/pattern-item">
    <xsl:variable name="myid" select="./@n"/>
    <xsl:for-each select="/transfer/section-def-cats/*">
      <xsl:if test="$myid=./@n">
	<xsl:value-of select="position()-1"/>
      </xsl:if>
    </xsl:for-each>
    <xsl:value-of select="string(' ')"/>
  </xsl:for-each>
  <xsl:value-of select="string('&#xA;')"/>
  </xsl:for-each>
</xsl:template>
</xsl:stylesheet>
