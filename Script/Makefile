#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
SUBDIR=Lua \

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
	 -I$(PROJBASE)/Log/include \
	 -I$(PROJBASE)/DesignPattern/include

STATIC_LIB= $(PROJBASE)/.lib/libLog.a \
	   $(PROJBASE)/.lib/libConfig.a

SHARED_LIB_DIR=
SHARED_LIB= -lcrypt -lrt -pthread -ldl

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.boost.thread.linux
include $(PROJBASE)/build/makefile.staticlib.boost.log.linux
include $(PROJBASE)/build/makefile.staticlib.lua.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

