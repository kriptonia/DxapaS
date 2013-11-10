#!/bin/bash
# COMMANDS USER->SERVER
#	S (MESSAGE) //SEND NEW MESSAGE
#	G (LAST MESSAGE ID) //GET NEW MESSAGES
#	L (NICKNAME) //LOGIN
#	M (IP) //New match
#	C //CLEAR FILE
#	Q (reason) //quit

# COMMANDS SERVER->USER
#	R (NUMBER OF NEW MESSAGES) //0-NO NEW MESSAGES 
#	(FROM) (MESSAGE)




#START SEVER execute: socat TCP4-LISTEN:9999,fork,tcpwrap=script EXEC:./server.sh
#or INETD superserver
#http    stream  tcp     nowait  albert  /home/albert/projecte/chat-server/server.sh server.sh
#LONGITUD DEL MENSAJE 32 (32caracters en horitzontal caben a la DS) BUFFER 34 32+\n+fi_string
MESSAGE_FILE=/home/albert/projecte/chat-server/messages
CONNECTED_FILE=/home/albert/projecte/chat-server/nicknames
NICKNAME=""

function send {
	local OUT=$(echo $1 | tr -d \\n | cut -c -32)
	while [ $(echo "$OUT" | wc -m) -lt 32 ]; do
		OUT="$OUT "
	done
	echo "$OUT"
}

if [ ! -e "$MESSAGE_FILE" ]; then
	echo -n > $MESSAGE_FILE
fi

#send "W Benvingut al XapaDS"
while [ "$NICKNAME" == "" ] || [ "$NICKNAME" == "-match" ]; do
	read command param
	if [ $? -ne 0 ]; then
	exit
	fi
	if [ "$command" == "L" ]; then
		NICKNAME=$(echo $param | cut -c-5)
	fi
done
LAST_MESSAGE_SENT=$(cat $MESSAGE_FILE | wc -l)
send  "-$NICKNAME s'ha conectat" >> $MESSAGE_FILE
#echo "R 0"
while [ 1 ]
do
	read command param
	if [ $? -ne 0 ] || [ "$command" == "Q" ]; then
		send  "-$NICKNAME ha desconectat" >> $MESSAGE_FILE
		exit
		#Esborrar el nick de la llista de nicks
		#a=:,,,:
		#b=":M Angels Moncusi:"
		#sed -i.bkg "/amoncusi/ s/$a/$b/" /etc/passwd
		#Fa una copia del fitxer /etc/passwd sobre /etc/passwd.bkg i modifica el fitxer /etc/passwd substituint en la línia on troba amoncusi el contingut de la variable a pel contingut de la variable b
	elif [ "$command" == "G" ]; then
		LAST_MESSAGE=$(cat $MESSAGE_FILE | wc -l)
		if [ $LAST_MESSAGE -lt $LAST_MESSAGE_SENT ]; then
			LAST_MESSAGE_SENT=$LAST_MESSAGE
		fi
		let N2SEND=$LAST_MESSAGE-$LAST_MESSAGE_SENT
		send "R $N2SEND"
		tail -n $N2SEND $MESSAGE_FILE
		LAST_MESSAGE_SENT=$LAST_MESSAGE
	elif [ "$command" == "S" ]; then
		send "$NICKNAME $param" >> $MESSAGE_FILE
	elif [ "$command" == "C" ]; then
		echo -n > $MESSAGE_FILE
	elif [ "$command" == "M" ]; then
		send "-match $param $NICKNAME" >> $MESSAGE_FILE
	fi
done
