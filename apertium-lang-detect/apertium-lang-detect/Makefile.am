h_sources = udm_common.h udm_guesser.h udm_crc32.h apertium-lang-detect-config.h
c_sources = crc32.c guesser.c

library_includedir = $(includedir)/$(GENERIC_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

bin_PROGRAMS = apertium-lang-detect

instdir = apertium-lang-detect

lib_LTLIBRARIES = libapertium-lang-detect-1.0.la
libapertium_lang_detect_1_0_la_SOURCES = $(h_sources) $(c_sources)
libapertium_lang_detect_1_0_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE) 

apertium_lang_detectdir = $(prefix)/apertium-lang-detect-$(GENERIC_API_VERSION)
apertium_lang_detectinclude = $(prefix)/include/apertium-lang-detect-$(GENERIC_API_VERSION)
apertium_lang_detectlib = $(prefix)/lib 

apertium_lang_detect_SOURCES = apertium-lang-detect.C
apertium_lang_detect_LDADD = $(DETLANG_LIBS) -lapertium-lang-detect-$(GENERIC_API_VERSION)
apertium_lang_detect_DATA = all_maps/* indicators/*

INCLUDES=-I$(top_srcdir) $(DETLANG_CFLAGS)
CLEANFILES = *~ 

EXTRA_DIST= all_maps/* indicators/*
