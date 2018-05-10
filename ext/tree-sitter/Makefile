SRCDIR = tree-sitter/src
EXTERNALSDIR = tree-sitter/externals
OUTDIR = out
SOURCE_FILES = $(wildcard $(SRCDIR)/runtime/*.c) $(wildcard $(EXTERNALSDIR)/utf8proc/utf8proc.c)
OUTFILES = $(wildcard $(OUTDIR)/*.o)
STATIC=libruntime.a

CFLAGS = -fPIC -std=c99 -Wall -O2
INC = -I$(SRCDIR) -I$(SRCDIR)/../include  -I$(EXTERNALSDIR)/utf8proc

OBJECT_FILES = $(SOURCE_FILES:%.c= %.o)

$(OBJECT_FILES): %.o: %.c
	$(CC) -c $(INC) -o $(OUTDIR)/$(notdir $@) $< $(CFLAGS)

$(STATIC):
	@echo "[Link (Static)]"
	@ar rcs $(OUTDIR)/$(STATIC) $(OUTFILES)

runtime: $(OBJECT_FILES) $(STATIC)

.PHONY: clean
clean:
	rm -f $(OUTDIR)/*.o $(OUTDIR)/*.a
