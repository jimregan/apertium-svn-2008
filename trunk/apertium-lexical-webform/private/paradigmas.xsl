<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="text" encoding="iso-8859-1"/>
   
   <xsl:template match="/form">
     <xsl:for-each select="./*">   
       <xsl:value-of select="./@PoS"/>
       <xsl:value-of select="string('_')"/>
       <xsl:value-of select="../@lang"/>
       <xsl:value-of select="string(':')"/>
       <xsl:apply-templates select="endings"/>
       <xsl:apply-templates select="paradigms"/>
       <xsl:value-of select="string('&#x9;')"/>
       <xsl:value-of select="./@nbr"/>       
       <xsl:value-of select="string('&#x9;')"/>
       <xsl:value-of select="./@gen"/>
       <xsl:value-of select="string('&#xA;')"/>
     </xsl:for-each>
   </xsl:template>

   <xsl:template match="endings">
<!--     <xsl:value-of select="./stem"/>  -->
     <xsl:apply-templates select="./stem"/>
     <xsl:for-each select="./ending">
       <xsl:if test="not(position()=1)">
	 <xsl:value-of select="string(' ')"/>
       </xsl:if>
       <xsl:if test="not(.=string(''))">
	 <xsl:value-of select="string('/')"/>
       </xsl:if>
       <xsl:value-of select="."/>
     </xsl:for-each>
   </xsl:template>

   <xsl:template match="paradigms">
     <xsl:value-of select="string('&#x9;')"/>
     <xsl:value-of select="./@howmany"/>
     <xsl:for-each select="./*">
       <xsl:value-of select="string('&#x9;')"/>
       <xsl:value-of select="./@n"/>
     </xsl:for-each>
   </xsl:template>

  <xsl:template match="b">
     <xsl:value-of select="string(' ')"/>
  </xsl:template>
 
</xsl:stylesheet> 
