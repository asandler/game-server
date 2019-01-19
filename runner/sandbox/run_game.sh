#!/bin/bash

# $1 - game
# $2 - player1
# $3 - player2
# $4 - sec
# $5 - usec
# $6 - run_dir
# $7 - name1
# $8 - name2
# $9 - config_number

mkdir -p "$6"

CONFIG_NUMBER=""$9""
TOTAL_CONFIG_NUMBER=$(ls -1 ../games/""$1""/configs/ | wc -l)

if [ "$CONFIG_NUMBER" == "" ]; then
    CONFIG_NUMBER="1"
else
    if [ "$CONFIG_NUMBER" == "random" ]; then
        CONFIG_NUMBER=$(seq 1 "$TOTAL_CONFIG_NUMBER" | sort -R | head -n 1)
    fi
fi

echo "game: "$1"" > "$6"/play.output
echo "player1: "$7"" >> "$6"/play.output
echo "player2: "$8"" >> "$6"/play.output

echo "game: "$1"" > "$6"/play.err
echo "player1: "$7"" >> "$6"/play.err
echo "player2: "$8"" >> "$6"/play.err

if [ "$CONFIG_NUMBER" -gt "$TOTAL_CONFIG_NUMBER" ]; then
    echo "ERROR: bad config number" >> "$6"/play.output
    echo "ERROR: bad config number" >> "$6"/play.err
fi

./battle "$2" "$3" ../games/"$1"/checker ../games/"$1"/configs/"$CONFIG_NUMBER"/init1 ../games/"$1"/configs/"$CONFIG_NUMBER"/init2 "$4" "$5" >> "$6"/play.output 2>> "$6"/play.err
