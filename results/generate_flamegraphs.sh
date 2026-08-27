#!/bin/bash
BASE=/home/akhenaton/iot-nfc-sim/loopascon

for file in akhenaton_profile.collapsed nefertiti_profile.collapsed
do
    if [ -f "$BASE/$file" ]; then
        echo "Generating FlameGraph for $file..."
        ~/FlameGraph/flamegraph.pl "$BASE/$file" > "$BASE/${file%.collapsed}.svg"
    else
        echo "Missing profile: $BASE/$file"
    fi
done
