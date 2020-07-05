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
	 -I$(PROJBASE)/Utility/include \
	 -I$(PROJBASE)/Config/include \
	 -I$(PROJBASE)/Processor/include \
	 -I$(PROJBASE)/DesignPattern/include

STATIC_LIB=

SHARED_LIB_DIR=
SHARED_LIB=

include $(PROJBASE)/build/makefile.rapidxml.header
include $(PROJBASE)/build/makefile.staticlib.configAndLog.linux

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

