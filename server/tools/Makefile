XSLTP = xsltproc
DOT = dot

XSLT += xsl/conf.group.xsl
XSLT += xsl/conf.params.xsl
XSLT += xsl/conf.properties.xsl
XSLT += xsl/conf.resolve.xsl
XSLT += xsl/conf.dot.xsl

CONF = ../conf/services.xml

all: services.png

services.png: $(CONF) $(XSLT)
	$(XSLTP) xsl/conf.dot.xsl $< | $(DOT) -Tpng > $@
	@ls -lh $@

clean:
	$(RM) services.png

.PHONY: all clean
