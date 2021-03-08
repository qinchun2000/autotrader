#! /bin/bash

PROC_NAME=mdservice
ProcNumber=`ps -ef |grep -w $PROC_NAME|grep -v grep|wc -l`
if [ $ProcNumber -le 0 ];then
   echo "$PROC_NAME is not run"
else
   echo "$PROC_NAME is  running.."
ps -ef | grep $PROC_NAME | grep -v grep | cut -c 9-15 | xargs kill -9
fi

