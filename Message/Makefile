#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
SUBDIR=

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR= -I. -IAutoMsg -IManMsg \
		 -I$(PROJBASE)/DesignPattern/include \
		 -I$(PROJBASE)/Log/include

STATIC_LIB=$(PROJBASE)/.lib/libLog.a \
		   $(PROJBASE)/.lib/libConfig.a

SHARED_LIB_DIR=
SHARED_LIB= -pthread

include $(PROJBASE)/build/makefile.boost.header
include $(PROJBASE)/build/makefile.staticlib.boost.thread.linux
include $(PROJBASE)/build/makefile.staticlib.boost.log.linux
#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules
