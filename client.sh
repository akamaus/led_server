#!/bin/bash

SERV_INPUT="/tmp/led_input"
SERV_OUTPUT="/tmp/led_output"

case $1 in
    "get-led-state")
        echo "get-led-state" > $SERV_INPUT
        cat $SERV_OUTPUT | grep OK | cut -d \  -f 2 || exit 1;;
    "set-led-state")
        echo "set-led-state" $2 > $SERV_INPUT
        cat $SERV_OUTPUT | grep FAILED && exit 1;;
    *) echo "Unknown command"
esac

