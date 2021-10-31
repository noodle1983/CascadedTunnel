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
INC_DIR= -I. -Iinclude \
		-I../DesignPattern/include \
		-I../Net/include \
		-I../Log/include \
		-I../Data/include \
		-I../Utility/include \
		-I../Processor/include

STATIC_LIB= \
    $(PROJBASE)/.lib/libProcessor.a \
	$(PROJBASE)/.lib/libLog.a \
	$(PROJBASE)/.lib/libNet.a \
	$(PROJBASE)/.lib/libUtility.a \
    $(PROJBASE)/.lib/libConfig.a

SHARED_LIB_DIR=
SHARED_LIB=-pthread

include $(PROJBASE)/build/makefile.staticlib.libevent.linux
include $(PROJBASE)/build/makefile.staticlib.tc_malloc.linux
#########################################
#              compiler                 #
#########################################
include $(PROJBASE)/build/makefile.compiler

#########################################
#              rules                    #
#########################################
include $(PROJBASE)/build/makefile.compile.rules

