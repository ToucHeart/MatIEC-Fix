#!/bin/bash

for file in PLCOPEN/2.4_Variables/*.txt; 
do
    if [ -e "$file" ]; 
    then
        filename=$(basename -- "$file")

        echo -e  "$filename"

        ./iec2c "$file" -T output;
        
        if [ $? -eq 0 ]; then
            echo "Test for $filename succeeded."
        else
            echo "Test for $filename failed."
        fi

        echo -e "\n\n"
    fi
done
