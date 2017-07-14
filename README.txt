terra README
===============

 terra is a service to control electrical (radio) outlets, based on clock, periodically and/or hygrometer (temperature / humidity) scheduling


Prerequisites
===============

 terra is designed to run on Raspberry Pi 2/3 and Gentoo GNU/Linux.

 terra depends on
 	- OpenRC
 	- ncursesw 5+ (wide character support)
 	- syslog
 	- libconfig


Installation
===============

 git clone https://github.com/sellep/terrap.git
 cd terrap
 make && sudo make install

 `make install` will copy the default configuration file (res/terra.conf) to /etc/terra/terra.conf, the init script (res/terra.init.d) to /etc/init.d/terra and the binary to /usr/local/bin.

 IMPORTANT:
 Please check the configuration file, you might want to modify the BCM GPIO pins and other settings.