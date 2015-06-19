#!/bin/bash

SERV_INPUT="/tmp/led_input"
SERV_OUTPUT="/tmp/led_output"

function getter() {
    echo $1 > $SERV_INPUT
    cat $SERV_OUTPUT | grep OK | cut -d \  -f 2 || exit 1
}

function setter() {
    echo $1 $2 > $SERV_INPUT
    cat $SERV_OUTPUT | grep FAILED && exit 1
}

case $1 in
    "get-led-state") getter $1;;
    "set-led-state") setter $1 $2;;
    "get-led-color") getter $1;;
    "set-led-color") setter $1 $2;;
    "get-led-rate")  getter $1;;
    "set-led-rate")  setter $1 $2;;

    *) echo "Unknown command"
esac

