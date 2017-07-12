#!/sbin/openrc-run

depend() {
	use logger
}

start() {
	ebegin "Starting terra service"

	start-stop-daemon --start --background --exec /usr/local/bin/terrad --pidfile /var/run/terra --make-pidfile

	eend $?
}

stop() {
	ebegin "Stopping terra service"

	start-stop-daemon --stop --signal TERM --retry 10 --exec /usr/local/bin/terrad --pidfile /var/run/terra

	eend $?
}
