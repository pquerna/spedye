BUILDTYPE ?= Debug

all: out/Makefile
	$(MAKE) -C out V=1 BUILDTYPE=$(BUILDTYPE) -j4
	-ln -fs out/$(BUILDTYPE)/spedye spedye

out/Release/monitoring-agent: all

out/Makefile:
	./configure

clean:
	rm -rf out

distclean:
	rm -rf out

VERSION=$(shell git describe)
TARNAME=spedye-$(VERSION)

dist:
	git archive --format=tar --prefix=$(TARNAME)/ HEAD | tar xf -
	tar -cf $(TARNAME).tar $(TARNAME)
	rm -rf $(TARNAME)
	gzip -f -9 $(TARNAME).tar

update:
	git submodule foreach git fetch && git submodule update --init --recursive

.PHONY: clean dist distclean all test tests
