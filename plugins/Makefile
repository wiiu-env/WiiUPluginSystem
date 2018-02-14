SUBDIRS = $(shell ls -d */)

.PHONY: all clean

all:
	for dir in $(SUBDIRS); do \
	$(MAKE) -C $$dir -f Makefile; \
	done
    
clean:
	for dir in $(SUBDIRS); do \
	$(MAKE) -C $$dir -f Makefile clean; \
	done