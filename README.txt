terra README
===============
terra is a service to control electrical (radio) outlets, based on clock, periodically and/or hygrometer (temperature / humitidy) scheduling

Prerequisites
===============
terra is designed to run on Raspberry Pi 2/3 and Gentoo GNU/Linux.
It is possible to bypass the Gentoo prerequisite.

terra depends on
	- OpenRC
	- ncursesw 5+
	- syslog
	- libconfig

Installation
===============
git clone https://github.com/sellep/terrap.git
cd terrap
make && sudo make install

`make install` will copy the default configuration file (res/terra.conf) to /etc/conf.d/terra, the init script (res/terra.init.d) to /etc/init.d/terra and the binary to /usr/local/bin.

IMPORTANT:
Please check the configuration file, you might want to modify the BCM GPIO pins and other settings.