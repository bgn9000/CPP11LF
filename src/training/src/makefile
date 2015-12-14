DIRS:=$(shell ls -d */)

.PHONY: all clean

all:
	@for dir in $(DIRS); do  \
	    make -C $$dir || exit 1;    \
	done

clean:
	@for dir in $(DIRS); do  \
	    make -C $$dir clean || exit 1;    \
	done
