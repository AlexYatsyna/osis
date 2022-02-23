#!/bin/bash

start=$(date +%s%3N)
eval $1
end=$(date +%s%3N)
duration=$((end - start))

echo -e "Command: $1 \nDuration: $duration ms"
