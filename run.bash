#!/bin/bash
# DO NOT MODIFY THIS FILE

# This script compiles and runs the webserver.

c++ -o src/serveMessage src/main.cpp

src/serveMessage 8080 "We will meet at midnight"
