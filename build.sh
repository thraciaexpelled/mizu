#!/usr/bin/env bash

set -e

# User options
CC=gcc
CC_FLAGS="-Wall -Wextra -Wpedantic -Wno-discarded-qualifiers -std=c11 -march=native -ggdb -O0 -Icli/ -Iclib-deps/ -Ilog/ -Iproject/"
PREFIX=/usr/local/bin
CMD_PREFIX="$CC $CC_FLAGS"

BIN=mizu
SUDO_BIN=sudo

build() {
	printf "	[cc] compiling clib stuff\n"
	$CMD_PREFIX -c -o clib_strdup.o clib-deps/strdup/strdup.c
	$CMD_PREFIX -c -o clib_ssw.o clib-deps/str-starts-with/str-starts-with.c
	$CMD_PREFIX -c -o clib_sew.o clib-deps/str-ends-with/str-ends-with.c
	$CMD_PREFIX -c -o clib_path.o clib-deps/path-join/path-join.c

	printf "	[cc] compiling logger\n"
	$CMD_PREFIX -c -o log.o log/log.c

	printf "	[cc] compiling config writer\n"
	$CMD_PREFIX -c -o config_writer.o project/config_writer.c

	printf "	[cc] compiling subcommand functionality\n"
	$CMD_PREFIX -c -o subc_new.o project/subcommands/new.c

	printf "	[cc] building cli\n"
	$CMD_PREFIX -o $BIN cli/main.c log.o subc_new.o clib_path.o clib_strdup.o clib_ssw.o clib_sew.o config_writer.o
}

install_mizu() {
	if [[ ! -f $BIN ]]; then
		printf "	[err] exitting upon unbuilt project\n" >&2
		exit 1
	fi

	printf "Note: This script uses 'sudo' by default. Pass '-s' with your sudo program to change this behaviour.\n"
	$SUDO_BIN install -m755 $BIN ./$BIN $PREFIX/$BIN
}

clean() {
	rm -f ./mizu
	rm -f ./*.o
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
				exit 1
		esac
	done
	shift $((OPTIND - 1))
}

main "$@"