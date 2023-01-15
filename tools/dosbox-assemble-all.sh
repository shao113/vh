#!/bin/sh
# Generate a batch file and run it in dosbox, transferring files as necessary

WORK_DIR=~/.wine/drive_c/misc/psxdev/aspsx
TOOLS_DIR=~/stuff
BAT_BASENAME=as.bat
BAT_FILE=$WORK_DIR/$BAT_BASENAME
AS_EXE="AS-33.EXE"

LnkToObj () {
	BASE_NAME=${1%%.*}
	$TOOLS_DIR/convert-psyq-lnk.sh $BASE_NAME.c.o.lnk $BASE_NAME.c.o
}

echo "Generating $BAT_FILE..." 

#echo "cd /D c:/misc/psxdev/aspsx/" > $BAT_FILE
#echo "" > $BAT_FILE
echo "del ASMLOG.TXT" > $BAT_FILE
I=0
for ARG; do
	if [[ $(grep -Fi nonmatchings "$ARG") ]]; then
		echo "Skipping: $ARG" 
		continue
	fi
	((I++))
	cp "$ARG" "$WORK_DIR/T_$I.S"
	unix2dos "$WORK_DIR/T_$I.S"
	#echo "$AS_EXE -o T_$I.LNK T_$I.S" >> $BAT_FILE
	echo "$AS_EXE -o T_$I.LNK T_$I.S >> ASMLOG.TXT" >> $BAT_FILE
done
#echo "pause" >> $BAT_FILE
echo "exit" >> $BAT_FILE

echo "Running dosbox..."
SDL_VIDEODRIVER=dummy dosbox > /dev/null 2>&1 -c "mount c ${WORK_DIR}" -c "c:" -c "call ${BAT_BASENAME}"
#dosbox > /dev/null 2>&1 $BAT_FILE

echo "Generating object files..."
I=0
for ARG; do
	[[ $(grep -Fi nonmatchings "$ARG") ]] && continue
	((I++))
	TARGET="${ARG%.*}".o.lnk
	mv "$WORK_DIR/T_$I.LNK" "$TARGET"
	rm "$WORK_DIR/T_$I.S"
	LnkToObj "$TARGET"
done
