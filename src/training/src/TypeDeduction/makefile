ifneq   "$(gccversion)" ""
    GCC=$(gccversion)
else 
    GCC=g++-5
endif

.PHONY: all clean

SOURCES:=$(shell ls *.cpp)
TARGETS:=$(SOURCES:.cpp=.out)

CFLAGS=-m64 -fPIC
CFLAGS += -std=c++14 -D_REENTRANT -DLinux -Wno-unused-but-set-variable -Wno-unused-variable

ifneq "" "$(findstring $(sanitize), 1 on On ON)"
    CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=signed-integer-overflow -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=null 
endif

MYLIBS += -latomic -lz -lnsl -lpthread -ldl -lrt

all: $(TARGETS)

clean:
	rm -f *.out

%.out : %.cpp
	$(GCC) -fbuiltin -Wall -Werror -o $@ $< $(INCS) $(CFLAGS)

