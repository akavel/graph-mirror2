<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:import href="./db_htmlchunk.xsl"/>
  <xsl:import href="./defaults.xsl"/>

  <xsl:param name="suppress.navigation" select="0"/>
  <xsl:param name="toc.section.depth" select="1"/>
  <xsl:param name="generate.section.toc.level" select="0"/>

  <xsl:param name="img.src.path">images/</xsl:param>
  <xsl:param name="glossterm.auto.link" select="1"/>
  <xsl:param name="chunk.section.depth" select="1"/>
  <xsl:param name="example.autolabel" select="0"/>
  <xsl:param name="html.stylesheet">styles_html.css</xsl:param>
  <xsl:param name="use.id.as.filename" select="1"/>
  <xsl:param name="chunk.quietly" select="1"/>
  <xsl:param name="chunk.first.sections" select="1"/>
  <xsl:param name="ignore.image.scaling" select="1"/>
  <xsl:param name="generate.index" select="1" />
  <xsl:param name="navig.showtitles" select="1" />

  <xsl:template match="markup[@role = 'fo']" />

  <!-- Show "Translator:" followed by the list of translators, one on each line -->
  <xsl:template match="othercredit" mode="titlepage.mode">
    <xsl:variable name="contrib" select="string(contrib)"/>
    <xsl:if test="not(preceding-sibling::othercredit[string(contrib)=$contrib])">
      <xsl:apply-templates mode="titlepage.mode" select="contrib"/>
      <br/>
    </xsl:if>
    <xsl:call-template name="person.name"/>
  </xsl:template>

</xsl:stylesheet>
