ifneq   "$(gccversion)" ""
    GCC=$(gccversion)
else 
    GCC=g++-4.9
endif

SOURCES:=$(shell ls *.cpp)
TARGETS:=$(SOURCES:.cpp=.out)

CFLAGS=-m64 -fPIC
CFLAGS += -std=c++1y -D_REENTRANT -DLinux -pedantic-errors -Wconversion -Wno-unused-but-set-variable -Wno-unused-variable

ifneq "" "$(findstring $(sanitize), 1 on On ON)"
    CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=signed-integer-overflow -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=null 
endif

#MYLIBS += -latomic -lz -lnsl -lpthread -ldl -lrt
MYLIBS += -lpthread

all: $(TARGETS)

%.out : %.cpp
	$(GCC) -fbuiltin -Wall -Werror -o $@ $< $(INCS) $(CFLAGS) $(MYLIBS)

