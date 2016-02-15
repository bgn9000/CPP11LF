ifneq   "$(gccversion)" ""
    GCC=$(gccversion)
else 
    GCC=g++-5
endif

.PHONY: all clean

SOURCES:=$(shell ls *.cpp)
TARGETS:=$(SOURCES:.cpp=.out)

CFLAGS=-O2 -m64 -fPIC
CFLAGS += -std=c++14 -D_REENTRANT -DLinux -pedantic-errors -Wconversion -Wno-unused-but-set-variable -Wno-unused-variable -ftemplate-depth=1500 

ifneq "" "$(findstring $(sanitize), 1 on On ON)"
    CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=signed-integer-overflow -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=null 
endif

#MYLIBS += -latomic -lz -lnsl -lpthread -ldl -lrt
MYLIBS += -lpthread

all: $(TARGETS)

clean:
	rm -f *.out

%.out : %.cpp
	$(GCC) -fbuiltin -Wall -Werror -o $@ $< $(INCS) $(CFLAGS) $(MYLIBS)

