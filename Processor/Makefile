#########################################
#                top dir                #
#########################################
PROJBASE=$(CURDIR)/..

#########################################
#                target                 #
#########################################
SUBDIR=src 

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR=-I. \
     -I./include \
     -I./src \
	 -I$(PROJBASE)/DesignPattern/include \
	 -I$(PROJBASE)/Net/include \
	 -I$(PROJBASE)/Data/include \
	 -I$(PROJBASE)/Utility/include \

STATIC_LIB= \
	$(PROJBASE)/.lib/libUtility.a \

SHARED_LIB_DIR=
SHARED_LIB= -pthread

include $(PROJBASE)/build/makefile.staticlib.configAndLog.linux
include $(PROJBASE)/build/makefile.staticlib.libevent.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

