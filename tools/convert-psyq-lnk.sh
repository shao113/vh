#!/usr/bin/env bash
echo "running convert-psyq-lnk.sh"

if [[ $# != 2 ]]; then
	echo "#args: 1:lnk 2:obj"
	exit 1
fi

ASM="${2%%.*}.c.s"
if ! [[ -f "$ASM" ]]; then
	echo "missing .s"
	exit 1
fi

echo "running psyq-preprocess for $1..."
python3.10 ~/stuff/psyq-preprocess.py "$1" "$1"

echo "running psyq-obj-parser for $1..."
psyq-obj-parser "$1" -d -o "$2" | grep "^    HI16" > "$2-hi16.txt"
#psyq-obj-parser "$1" -o "$2" >/dev/null
#mips-elf-objdump -x "$2" > "$2-dump.txt"

echo "running elf-fixup for $2..."
python3.10 ~/stuff/elf-fixup.py "$2"

echo "running jtbl-fixup for $2..."
python3.10 ~/stuff/jtbl-fixup.py "$ASM" "$2"

echo "running addend-fixup for $2..."
python3.10 ~/stuff/addend-fixup.py "$2-hi16.txt" "$2"

echo "running markup-sym-names for $2..."
python3.10 ~/stuff/markup-sym-names.py "$2"
