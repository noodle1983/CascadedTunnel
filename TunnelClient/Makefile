PROJBASE=$(CURDIR)/..


#########################################
#                Target                 #
#########################################
SUBDIR=src \

#########################################
#      header and lib dependancy        #
#########################################
INC_DIR= -I. \
     -I$(PROJBASE)/TunnelClient/src \
     -I$(PROJBASE)/Net/include \
     -I$(PROJBASE)/Message/include \
     -I$(PROJBASE)/DesignPattern/include \
     -I$(PROJBASE)/Processor/include \
     -I$(PROJBASE)/Log/include \
     -I$(PROJBASE)/Data/include \
     -I$(PROJBASE)/Utility/include \

STATIC_LIB= \
    $(PROJBASE)/.lib/libTunnelClient.a \
    $(PROJBASE)/.lib/libProcessor.a \
    $(PROJBASE)/.lib/libLog.a \
    $(PROJBASE)/.lib/libNet.a \
    $(PROJBASE)/.lib/libConfig.a \
    $(PROJBASE)/.lib/libUtility.a \

SHARED_LIB_DIR=
SHARED_LIB=

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

