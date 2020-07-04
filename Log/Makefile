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

STATIC_LIB= $(PROJBASE)/.lib/libConfig.a \
	$(PROJBASE)/.lib/libProcessor.a \

SHARED_LIB_DIR=
SHARED_LIB= -pthread

include $(PROJBASE)/build/makefile.rapidxml.header

#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

