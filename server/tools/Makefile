XSLTP = xsltproc
DOT = dot

GIT_ID = $(shell git describe --always --tags 2>/dev/null)

CONF_XSLT += xsl/conf.group.xsl
CONF_XSLT += xsl/conf.params.xsl
CONF_XSLT += xsl/conf.properties.xsl
CONF_XSLT += xsl/conf.resolve.xsl
CONF_XSLT += xsl/conf.dot.xsl

CONF = ../conf/services.xml

all: services.png home8-sqlite.sql home8-postgre.sql

services.png: $(CONF) $(CONF_XSLT)
	$(XSLTP) xsl/conf.dot.xsl $< | $(DOT) -Tpng > $@
	@ls -lh $@

SCHEMA_XSL += xsl/schema.common.xsl
SCHEMA_XML = ../schema

%-sqlite.sql: $(SCHEMA_XML)/%.xml $(SCHEMA_XSL) xsl/schema.sqlite.xsl
	$(XSLTP) --stringparam codebase.version "$(GIT_ID)" xsl/schema.sqlite.xsl $<
%-postgre.sql: $(SCHEMA_XML)/%.xml $(SCHEMA_XSL) xsl/schema.postgre.xsl
	$(XSLTP) --stringparam codebase.version "$(GIT_ID)" xsl/schema.postgre.xsl $<

clean:
	$(RM) services.png
	$(RM) home8-sqlite.sql
	$(RM) home8-postgre.sql

.PHONY: all clean
