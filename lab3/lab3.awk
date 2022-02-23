#!/bin/awk -f

NF==3 \
&& $1~/^[А-Яа-яa-zA-Z]*$/ \
&& $2~/^[1-9][0-9]*$/ \
&& $3~/^[0-9]*(.[0-9]+)?$/ \
{
count[$1]+=$2; 
all_sum[$1]+=$3
}

END {
    for (item in count){
	print("\n")
        print(item ":")
	print("Count: " count[item])
	print("Average price: " all_sum[item]/count[item])
	print("All sum of item: " all_sum[item])
}
}
