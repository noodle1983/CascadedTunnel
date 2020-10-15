#!/bin/bash

SELF_NAME="${BASH_ARGV[0]}"
if [ "$SELF_NAME" == "" ]; then
    SELF_PATH=$0
    if [ ${SELF_PATH:0:1} != "/" ]; then 
        SELF_PATH="$PWD/$SELF_PATH"
    fi
    cat <<END_OF_USAGE
usage:
    敲 ". $SELF_PATH" 回车
    或者加入bashrc：echo ". $SELF_PATH" >> ~/.bashrc
    一些命令会改变目录，用命令"cd -"切回    
    gdbxx 已经设置好参数，可以直接r
END_OF_USAGE
    exit -1
fi
SELF_DIR=`dirname $SELF_NAME`
if [ ${SELF_DIR:0:1} != "/" ]; then 
    SELF_DIR="$PWD/$SELF_DIR"
fi

USER=`whoami`
USER_ID=`id -u`
PROJ_BASE=$SELF_DIR
echo "PROJ_BASE:$PROJ_BASE"

alias psmy="ps -ef | egrep \"^($USER|$USER_ID)\" | grep "
alias cdr="cd $PROJ_BASE"
alias cdc="cd $PROJ_BASE/TunnelClient/UnitTest/"
alias cds="cd $PROJ_BASE/TunnelServer/UnitTest/"
