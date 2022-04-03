#!/bin/env bash




PreCalculate="pi=3.14159265358979323846;\
              e=2.71828182845904523536;\
              minute=60;\
              hour=minute*60;\
              day=hour*24;\
              year=day*365.25;\
              epoch=$(date +%s)"

echo "$* =$(calc "$PreCalculate;$*" 2>/dev/null || echo 'calculation error.')"