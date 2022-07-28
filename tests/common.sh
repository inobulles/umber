#!/bin/sh
set -e

clear_="\033[0m"
regular="\033[0;"
bold="\033[1;"

purple="35m"
red="31m"
yellow="33m"
green="32m"
blue="34m"
grey="37m"

func_name=_$(openssl rand -hex 6)
