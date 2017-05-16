bin_PROGRAMS = macs2

macs2_SOURCES	=	macs2.c 
macs2_SOURCES	+= 	andl-lexer.l andl-parser.y 
macs2_SOURCES	+=	util.h util.c

macs2_CFLAGS = $(SYLVAN_CFLAGS) $(XML_CFLAGS)
macs2_LDADD = $(SYLVAN_LIBS) $(XML_LIBS)

macs2_YFLAGS   = -d

EXTRA_DIST  = andl-lexer.c andl-lexer.h
EXTRA_DIST += macs2-andl-parser.c macs2-andl-parser.h

BUILT_SOURCES  = andl-lexer.c andl-lexer.h
BUILT_SOURCES += macs2-andl-parser.c macs2-andl-parser.h

CLEANFILES  = andl-lexer.c andl-lexer.h
CLEANFILES += macs2-andl-parser.c macs2-andl-parser.h


