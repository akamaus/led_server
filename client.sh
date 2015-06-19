#!/bin/bash

SERV_INPUT="/tmp/led_input"
SERV_OUTPUT="/tmp/led_output"

set -o pipefail

function getter() {
    echo $* > $SERV_INPUT
    cat $SERV_OUTPUT | grep OK | cut -d \  -f 2 || exit 1
}

function setter() {
    echo $* > $SERV_INPUT
    cat $SERV_OUTPUT | grep FAILED && exit 1
}

case $1 in
    "get-led-state") getter $*;;
    "set-led-state") setter $*;;
    "get-led-color") getter $*;;
    "set-led-color") setter $*;;
    "get-led-rate")  getter $*;;
    "set-led-rate")  setter $*;;

    *) echo "Unknown command"
esac

