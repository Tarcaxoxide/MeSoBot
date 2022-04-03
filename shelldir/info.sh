#!/bin/env bash

echo "(OS:$(cat /etc/*-release|grep PRETTY_NAME|cut -d'=' -f2)) (KERNEL:$(uname -r)) (CPU:$(uname -pm))"
