#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/stackGrowDirect.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = stackGrowDirect.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

rtems:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)

all:showBacktrace

showBacktrace: showBacktrace.cpp
	g++ $< -o $@ -rdynamic -g -ldl
