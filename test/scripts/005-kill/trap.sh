#!/usr/bin/env bash

trap "exit 0" SIGTERM SIGINT

sleep 0.5
echo "You didn't actually kill the process" > err
exit 1