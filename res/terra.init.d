#!/sbin/openrc-run

depend() {
	need local
}

start() {
	ebegin "Starting terra daemon"

	start-stop-daemon --start --exec /usr/local/bin/terra --background -- daemon

	eend $?
}

stop() {
	ebegin "Stopping terra daemon"

	start-stop-daemon --stop --exec /usr/local/bin/terra

	eend $?
}
