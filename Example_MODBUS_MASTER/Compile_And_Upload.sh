#!/bin/bash
set -e

for f in SRC/*.c
do
    source ../../Framework/Tools/Compile_App.sh $f
done

# Remove unused files
rm -f Output/*.o Output/*.map Output/*.elf Output/*.hex

for f in Output/*.bin
do
    source ../../Framework/Tools/Upload_App.sh $f
done


echo;
echo;
echo ----------------------------------------------
echo ALL DONE!
echo ----------------------------------------------

read -p "Press enter to continue"