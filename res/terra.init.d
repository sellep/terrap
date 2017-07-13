#!/sbin/openrc-run

depend() {
	use logger
}

start() {
	ebegin "Starting terra service"

	start-stop-daemon --start --background --exec /usr/local/bin/terra --pidfile /var/run/terra --make-pidfile -- daemon

	eend $?
}

end() {
	ebegin "Stopping terra service"

	start-stop-daemon --stop --exec /usr/local/bin/terra --pidfile /var/run/terra --remove-pidfile

	eend $?
}
