#!/bin/bash

testFile()
{
    if [ -f $1 ];then
        echo $1 is a REG file
    else
        echo $1 is not a REG file
    fi
}
testDir()
{
    if [ -d $1 ]; then
        echo $1 is a dir
    else 
        echo $1 is not a dir
    fi
}
echo entry point here
for A in $*
    do
        testFile $A
        testDir $A
    done
