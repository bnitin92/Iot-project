#!/bin/sh

DAEMON_NAME="simpled1"

start() {
	printf "Starting $DAEMON_NAME: "
	/home/nitin/buildroot-2019.02.3/iot-project/project3/$DAEMON_NAME
	touch /var/lock/$DAEMON_NAME
	echo "OK"
}

stop() {
	printf "Stopping $DAEMON_NAME: "
	killall $DAEMON_NAME
	rm -f /var/lock/$DAEMON_NAME
	echo "OK"
}

restart() {
	stop
	start
}

case "$1" in
	start)
	start
	;;
	stop)
	stop
	;;
	restart|reload)
	restart
	;;
	*)
	echo "Usage: $0 {start|stop|restart}"
	exit 1
esac

exit $?
