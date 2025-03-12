#!/bin/bash

for i in $(seq 5 5 500)
do
    ./bin/main $i
done
