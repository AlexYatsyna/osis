#!/bin/bash
x=0
y=0
print(){
   for((i=0; i<3; i++))
   do
	if [ "${i}" -gt 0 ]
	then
		echo "-+-+-"
	fi
	for ((si=0; si<3; si++))
	do
	    local index=$(( i*3+si ))
	    local symbol="${game[$index]}"
            if [ -z "${symbol}" ]
            then
                symbol=" "
            fi
            if ((x==si && y==i))
            then
                echo -n -e "\e[1;47m$symbol"
                echo -n -e "\e[0m"
            else
                echo -n "$symbol"
            fi
	    if [ "${si}" -ne 2 ]
	    then
	       echo  -n "|"
	    fi
	done
	echo
    done 
}

check(){

    if expr "${game[0]}" : "${game[4]}" \& \
            "${game[0]}" : "${game[8]}" \& \
            "${game[0]}" >/dev/null
    then
            return 1
    fi

    if expr "${game[2]}" : "${game[4]}" \& \
            "${game[2]}" : "${game[6]}" \& \
            "${game[2]}" >/dev/null
    then
            return 1
    fi

    for (( i=0; i<3; i++))
    do
        if expr "${game[$((i*3))]}" : "${game[$((i*3+1))]}" \& \
                "${game[$((i*3))]}" : "${game[$((i*3+2))]}" \& \
                "${game[$((i*3))]}" >/dev/null
        then
            return 1
        fi
    done

    for (( i=0; i<3; i++))
    do
        if expr "${game[$((i))]}" : "${game[$((i+3))]}" \& \
                "${game[$((i))]}" : "${game[$((i+6))]}" \& \
                "${game[$((i))]}" >/dev/null
        then
            return 1
        fi
    done

    for (( i=0; i<3; i++))
    do
	for(( si=0; si<3; si++))
	do
	    local index=$(( i*3+si ))
            local symbol="${game[$index]}"
	    flag=1
            if [ -n "${symbol}" ]
            then
                flag=0
		return 0
            fi
	done
    done
    if [ "${flag}" -eq 0 ]
    then
        return 1
    fi

    return 0
}

show(){
   clear
   echo "<wasd> to move the cursor"
   echo "<Enter> to confirm"
   echo
   echo
   print
   echo
   echo
}

save(){
   for((i=0; i<3; i++))
   do
        if [ "${i}" -gt 0 ]
        then
                echo "-+-+-"
        fi
        for ((si=0; si<3; si++))
        do
            local index=$(( i*3+si ))
            local symbol="${game[$index]}"
            if [ -z "${symbol}" ]
            then
                symbol=" "
            fi
            echo -n "$symbol"
            if [ "${si}" -ne 2 ]
            then
               echo  -n "|"
            fi
        done
        echo
    done 
}>save.txt

turn(){
    check
    if [ "$?" -eq "1" ]
    then
        show
 	save
	play=""
    fi

    if expr "$play" : "O" >/dev/null
    then
        play=X
    elif expr "$play" : "X" >/dev/null
    then
        play=O
    fi
}


play=O
while [ -n "$play" ]
do
    show
    read -n 1 -r -p "Enter $play"
    case "$REPLY" in
    "w" )
        ((y = y ? y-1 : 0))
    ;;
    "a" )
        ((x = x ? x-1 : 0))
    ;;
    "s" )
        ((y = y==2 ? 2 : y+1))
    ;;
    "d" )
        ((x = x==2 ? 2 : x+1))
    ;;
    "`echo -e '\n'`" )
        index=$((y*3+x))
        if [ -z "${game[$index]}" ]
        then
            game[$index]=$play
            turn
        fi
    ;;
    "b" )
	save
    ;;
    esac

done
