h_sources = apertium-multiple-translations-config.h TransferMult.H
cpp_sources = TransferMult.C

library_includedir = $(includedir)/$(GENERIC_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

bin_PROGRAMS = apertium-multiple-translations

instdir = apertium-multiple-translations

lib_LTLIBRARIES = libapertium-multiple-translations-1.0.la
libapertium_multiple_translations_1_0_la_SOURCES = $(h_sources) $(cpp_sources)
libapertium_multiple_translations_1_0_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE) 

apertium_multiple_translationsdir = $(prefix)/detlang-$(GENERIC_API_VERSION)
apertium_multiple_translationsinclude = $(prefix)/include/detlang-$(GENERIC_API_VERSION)
apertium_multiple_translationslib = $(prefix)/lib 

apertium_multiple_translations_SOURCES = apertium-multiple-translations.C
apertium_multiple_translations_LDADD = $(APERTIUM_MULTIPLE_TRANSLATIONS_LIBS) -lapertium-multiple-translations-$(GENERIC_API_VERSION)
#apertium_multiple_translations_DATA =

INCLUDES=-I$(top_srcdir) $(APERTIUM_MULTIPLE_TRANSLATIONS_CFLAGS)
CLEANFILES = *~ 

man_MANS=apertium-multiple-translations.1 


