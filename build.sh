#!/usr/bin/env bash

set -e

# User options
CC=gcc
CC_FLAGS="-Wall -Wextra -Wpedantic -std=c11 -march=native -ggdb -O0 -Icli/ -Ilog/"
PREFIX=/usr/local/bin
CMD_PREFIX="$CC $CC_FLAGS"

BIN=mizu
SUDO_BIN=sudo

build() {
	printf "	[cc] compiling logger\n"
	$CMD_PREFIX -c -o log.o log/log.c

	printf "	[cc] building cli\n"
	$CMD_PREFIX -o $BIN cli/main.c log.o
}

install_mizu() {
	if [[ ! -f $BIN ]]; then
		printf "	[err] exitting upon unbuilt project\n" >&2
		exit -1
	fi

	printf "Note: This script uses 'sudo' by default. Pass '-s' with your sudo program to change this behaviour.\n"
	$SUDO_BIN install -m755 $BIN ./$BIN $PREFIX/$BIN
}

clean() {
	rm -f ./mizu
	rm -f *.o
}

main() {
	while getopts "bchis:" c; do
		case $c in
			b)
				build
				;;
			c)
				clean
				;;
			i)
				install_mizu
				;;
			s)
				[[ ! -z $OPTARG ]] && SUDO_BIN=$OPTARG
				;;
			h)
				echo -e "-b, -i =======> build and install packages"
				echo -e "-s [PROGRAM] => use different sudo program"
				echo -e "-h ===========> prints this message and exits"
				exit 0
				;;
			*)
				exit -1
		esac
	done
	shift $(($OPTIND - 1))
}

main $@