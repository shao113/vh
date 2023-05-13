#!/bin/sh
# Generate a batch file and run it in dosbox, transferring files as necessary
# (Extra-hacky version tailored to my setup)

WORK_DIR=~/.wine/drive_c/misc/psxdev/aspsx
TOOLS_DIR=~/stuff
BAT_BASENAME=as.bat
BAT_FILE=$WORK_DIR/$BAT_BASENAME
AS_EXE="AS-33.EXE"

echo "Generating $BAT_FILE..." 

echo "del ASMLOG.TXT" > $BAT_FILE
I=0
for ARG; do
	if [[ $(grep -Fi nonmatchings "$ARG") ]]; then
		echo "Skipping: $ARG" 
		continue
	fi
	((I++))
	BASENAMES_ARRAY+=("${ARG%%.*}")
	cp "$ARG" "$WORK_DIR/T_$I.S"
	unix2dos "$WORK_DIR/T_$I.S"
	echo "$AS_EXE -o T_$I.LNK T_$I.S >> ASMLOG.TXT" >> $BAT_FILE
done
echo "exit" >> $BAT_FILE

if [[ ! -z "${ASPSX_PERSISTENT}" ]] && [[ $# == 1 ]]; then
	echo "Syncing with dosbox..."

	# should have already ran setup-dosbox.sh, or:
	# socat -d -d pty,raw,echo=0 pty,raw,echo=0 &
	# dosbox > /dev/null 2>&1 -c "mount c: ~/.wine/drive_c/misc/psxdev/aspsx" -c "serial1 directserial realport:pts/1" -c "c:" -c "msk316.exe take k1" &
	
	#killall -CONT dosbox
	
	# hoping this is enough to flush any left-over buffer; maybe do this in a loop until empty value?
	read -t 0.001 < /dev/pts/0
	
	# send "a" to trigger assembler
	echo -n a > /dev/pts/0
	# wait for ok
	head -n 1 /dev/pts/0
	
	#killall -STOP dosbox
else
	echo "Running dosbox..."
	SDL_VIDEODRIVER=dummy dosbox > /dev/null 2>&1 -c "mount c ${WORK_DIR}" -c "c:" -c "call ${BAT_BASENAME}"
	#dosbox > /dev/null 2>&1 $BAT_FILE
fi

echo "Moving..."
I=0
for TARGET_BASENAME in "${BASENAMES_ARRAY[@]}"; do
	((I++))
	mv "$WORK_DIR/T_$I.LNK" "${TARGET_BASENAME}".c.o.lnk
	rm "$WORK_DIR/T_$I.S"
done

echo "Generating object files (parallelized)..."

printf -v LIST "%s\n" "${BASENAMES_ARRAY[@]}" # Array to NL-delimited string
LIST=${LIST%?} # Remove the final NL
# For each basename in LIST, run converter (parallelized to boost speed)
#echo "$LIST" | parallel -j4 SMOLPRINT=0 $TOOLS_DIR/convert-psyq-lnk.sh {}.c.o.lnk {}.c.o
echo "$LIST" | parallel $TOOLS_DIR/convert-psyq-lnk.sh {}.c.o.lnk {}.c.o
