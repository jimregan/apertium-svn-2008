#!/bin/sh

# Run this as root
# I use:
#   ./install_services $PWD /usr/local

services="org.apertium.info.service org.apertium.mode.service"

if [ -z $1 ]; then
	echo "usage: install_system_service <path to info.py and mode.py> <path to apertium>";
	exit 1
fi

for service in $services; do
	cat $service | sed -e "s,\@servdir\@,${1},g" | sed -e "s,\@prefix\@,${2},g" > /usr/share/dbus-1/services/$service
done


