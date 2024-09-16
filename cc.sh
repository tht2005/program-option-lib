#!/bin/bash

include_dir="./include/"

command="cc $1.c -Wall -I$include_dir -o$1"

echo execute: $command

$command

