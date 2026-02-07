#!/bin/bash

aseprite\
 --batch\
 --format json-array\
 --data ../frames.json\
 --sheet-type rows\
 --sheet-width 256\
 --sheet-height 256\
	*.aseprite\
 --sheet ../sprites.png\
 --list-tags\
 --list-slices
