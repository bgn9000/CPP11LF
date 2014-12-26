TARGET=cpp11lf
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
LIBS:=$(shell ls ../../$(LIB_DIR)/*.a)
ifneq "" "$(findstring $(CC64), 1 on On ON)"
   CFLAGS=-m64 -fPIC
else
   CFLAGS=-m32 -march=i686
endif
CFLAGS_OPTIM := $(GCC_MAXSPEED_OPTIMIZATION)
ifeq ($(GCC_VERSION_MINOR), 9)
	CFLAGS += -DC11 -std=c++11
else
	CFLAGS += -std=c++0x
endif

CFLAGS+=-D_REENTRANT -DLinux
ifneq "" "$(findstring $(gcov), 1 on On ON)"
	CFLAGS+=-fprofile-arcs -ftest-coverage
endif
ifneq "" "$(findstring $(debug), 1 on On ON)"
        CFLAGS += -g -ggdb -DDEBUG
else
        CFLAGS += -g -ggdb $(CFLAGS_OPTIM)
endif

MYLIBS += -lnsl -lpthread -ldl -lrt

all: $(TARGET)
	@rm -f $(OBJ_DIR)/main.o
depend:
	rm -f $(DEPENDS)
	$(MAKE) $(DEPENDS)

$(TARGET):$(OBJECTS) $(LIBS)
	g++ -Wall -o $@ $(OBJECTS) $(CFLAGS) $(MYLIBS)

fullclean: cleanlite
	rm -rf $(OBJ_DIR)

clean: cleanlite
	rm -f $(OBJ_DIR)/*.o

cleanlite:
	rm -f $(TARGET)

$(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(OBJ_DIR)
	g++ -fbuiltin -Wall -Werror -c -o $@ $< $(INCS) $(CFLAGS)

$(OBJ_DIR)/%.od : %.cpp
	@mkdir -p $(OBJ_DIR)
	g++ -MM -MT $(patsubst %.od, %.o, $@) $(INCS) $(CFLAGS) $< > $@

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "fullclean"
-include $(DEPENDS)
endif
endif
