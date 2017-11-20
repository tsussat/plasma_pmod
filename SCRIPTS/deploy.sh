#!/bin/sh

root=$( realpath "$( dirname "$0" )" )
hostname=$( hostname )

#ENABLE_BITSTREAM=
#ENABLE_SIMULATION=

RSYNC="rsync -a -s --progress --delete"
KEY="$HOME/.ssh/$hostname/$hostname"
SSH_OPTIONS="-o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no"
SSH="ssh"
SCP="scp"
GATEWAY="ssh.enseirb-matmeca.fr"
USER="paukocialkowsk"
PORT=1234
PROJECT="projet-avance-se"
BITSTREAM="Plasma/SYNTHESIS/top_plasma.bit"
BINARY="Plasma/test.bin"
SRC="$root/"
DST="$USER@$GATEWAY:$PROJECT"
HOST="$1"

if [ -z "$HOST" ]
then
	HOST="lsd"
fi

test=$( ssh-add -l | grep "$KEY" || true )

if [ -z "$test" ]
then
	echo -e "Adding SSH key for \e[1;33m$hostname\e[0m:"
	ssh-add "$KEY"
fi

if [ -z "$DRY" ] && [ -z "$NODRY" ] && [ -z "$NOSYNC" ]
then
	echo -en "Sync in \e[1;31mdry\e[0m mode: "
	read DRY
fi

if [ -n "$DRY" ]
then
	dry="--dry-run"
	dry_description=" \e[1;31m(dry run)\e[0m"
fi

echo -e "Deploy \e[1;34mplasma\e[0m from \e[1;33m$hostname\e[0m to \e[1;33m$GATEWAY\e[0m$dry_description"

if [ -z "$NOSYNC" ]
then
	$RSYNC $dry "$SRC" "$DST"
fi

echo -e "Setup \e[1;34mgateway\e[0m on port \e[1;33m$PORT\e[0m"

screen -S plasma -d -m ssh -L $PORT:$HOST:22 "$GATEWAY" && echo "Attached via screen" || exit 1
sleep 5

if [ -n "$ENABLE_SIMULATION" ]
then
	echo -e "Run \e[1;34msimulation\e[0m"

	$SSH $SSH_OPTIONS -p $PORT "$USER@localhost" "cd projet-avance-se && source config.sh && make -C Plasma"
fi

if [ -n "$ENABLE_BITSTREAM" ]
then
	echo -e "Run \e[1;34mfpga\e[0m"

	$SSH $SSH_OPTIONS -p $PORT "$USER@localhost" "cd projet-avance-se && source config.sh && make -C Plasma fpga MY_PROJECT=boot_loader"

	echo -e "Copy \e[1;34mbitstream\e[0m"

	$SCP "$DST/$BITSTREAM" "$SRC/$BITSTREAM"
	cp "$SRC/$BITSTREAM" $( basename "$SRC/$BITSTREAM" )
fi

echo -e "Run \e[1;34msend\e[0m"

$SSH $SSH_OPTIONS -p $PORT "$USER@localhost" "cd projet-avance-se && source config.sh && make -C Plasma send MY_PROJECT=hello"

echo -e "Copy \e[1;34mbinary\e[0m"

$SCP "$DST/$BINARY" "$SRC/$BINARY"
cp "$SRC/$BINARY" $( basename "$SRC/$BINARY" )

echo -e "Close \e[1;34mgateway\e[0m on port \e[1;33m$PORT\e[0m"

screen -X -S plasma kill
