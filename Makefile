# Include the basic Makefile template
include $(SKULL_SRCTOP)/.skull/makefiles/Makefile.cpp.inc

INC += \
    -Isrc \
    -I../../common/cpp/src \
    -I./deps/http-parser

DEPS_LDFLAGS += \
    -L../../common/cpp/lib \
    -L./deps/http-parser/

DEPS_LIBS += \
    -lprotobuf \
    -lskullcpp-api \
    -lhttp_parser \
    -Wl,--no-as-needed \
    -lskull-common-cpp

TEST_DEPS_LIBS += \
    -lprotobuf \
    -lhttp_parser \
    -lskull-common-cpp \
    -lskull-unittest-cpp \
    -lskull-unittest-c

# Objs and deployment related items
SRCS = \
    src/service.cpp \
    src/HttpResponseImp.cpp \
    src/HttpRequest.cpp \
    src/EPHandler.cpp

TEST_SRCS = \
    tests/test_service.cpp

# valgrind suppresion file
#  note: if the suppresion file is exist, then need to append
#        `--suppressions=$(SUPPRESSION)` to `VALGRIND`
SUPPRESSION := $(GLOBAL_SUPPRESSION)

# Default valgrind command
VALGRIND ?= valgrind --tool=memcheck --leak-check=full -v \
    --gen-suppressions=all --error-exitcode=1

# Include the basic Makefile targets
include $(SKULL_SRCTOP)/.skull/makefiles/Makefile.cpp.targets

build_http_parser:
	cd ./deps/http-parser && make package CFLAGS_FAST_EXTRA+=-fPIC

clean_http_parser:
	cd ./deps/http-parser && make clean

prepare: build_http_parser

clean: clean_http_parser

