#!/sbin/openrc-run

start() {
	ebegin "Starting terra service"

	start-stop-daemon --start --exec /usr/local/bin/terra --background -- daemon

	eend $?
}

stop() {
	ebegin "Stopping terra service"

	start-stop-daemon --stop --exec /usr/local/bin/terra

	eend $?
}
