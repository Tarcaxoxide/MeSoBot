#!/bin/env bash

PreCalculate='pi=3.14159265358979323846;e=2.71828182845904523536'

echo "$* =$(calc "$PreCalculate;$*")"