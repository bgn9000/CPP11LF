TARGET=cpp11lf
ifneq   "$(gccversion)" ""
    GCC=$(gccversion)
else 
    GCC=g++-4.9
endif
LIB_DIR=lib
OBJ_DIR=obj
ifneq "" "$(findstring $(CC64), 1 on On ON)"
    LIB_DIR+=64
    OBJ_DIR+=64
    TARGET+=64
endif

SOURCES:=$(shell ls *.cpp)
OBJECTS:=$(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
DEPENDS:=$(addsuffix d, $(OBJECTS))
ifneq "" "$(findstring $(CC64), 1 on On ON)"
   CFLAGS=-m64 -fPIC
else
   CFLAGS=-m32 -march=i686
endif
CFLAGS += -std=c++1y -D_REENTRANT -DLinux -Wno-unused-but-set-variable -Wno-unused-variable
ifneq "" "$(findstring $(gcov), 1 on On ON)"
	CFLAGS+=-fprofile-arcs -ftest-coverage
endif
ifneq "" "$(findstring $(debug), 1 on On ON)"
    CFLAGS += -g -ggdb -DDEBUG
else
    CFLAGS += -g -ggdb -O2
endif
ifneq "" "$(findstring $(sanitize), 1 on On ON)"
    CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=signed-integer-overflow -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=null 
endif

INCS = -I../include
ifneq "" "$(findstring $(blosc), 1 on On ON)"
    CFLAGS += -DUSE_BLOSC
    INCS += -I$(BLOSC_HOME)/include
    MYLIBS += -Wl,-Bstatic -L$(BLOSC_HOME)/lib -lblosc -Wl,-Bdynamic    
endif
MYLIBS += -latomic -lz -lnsl -lpthread -ldl -lrt

all: $(TARGET)
	@rm -f $(OBJ_DIR)/main.o
depend:
	rm -f $(DEPENDS)
	$(MAKE) $(DEPENDS)

$(TARGET):$(OBJECTS)
	$(GCC) -Wall -o $@ $(OBJECTS) $(CFLAGS) $(MYLIBS)

fullclean: cleanlite
	rm -rf $(OBJ_DIR)

clean: cleanlite
	rm -f $(OBJ_DIR)/*.o

cleanlite:
	rm -f $(TARGET)

$(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(OBJ_DIR)
	$(GCC) -fbuiltin -Wall -Werror -c -o $@ $< $(INCS) $(CFLAGS)

$(OBJ_DIR)/%.od : %.cpp
	@mkdir -p $(OBJ_DIR)
	$(GCC) -MM -MT $(patsubst %.od, %.o, $@) $(INCS) $(CFLAGS) $< > $@

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "fullclean"
-include $(DEPENDS)
endif
endif
