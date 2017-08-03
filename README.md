terra
===============

 terra is a service to control electrical (radio) outlets, based on clock, periodically or hygrometer data scheduling


Prerequisites
===============

 terra is designed to run on Raspberry Pi 2/3 and Gentoo GNU/Linux.

 terra depends on  
 	- OpenRC  
 	- ncursesw 5+ (wide character support, optional)  
 	- libconfig  


Installation
===============

 `git clone https://github.com/sellep/terrap.git && cd terrap && make && sudo make install`

 `make install` will copy the default configuration file (res/terra.conf) to /etc/terra/terra.conf, the init script (res/terra.init.d) to /etc/init.d/terra and the binary to /usr/local/bin.

 IMPORTANT:
 Please check the configuration file, you might want to modify the BCM GPIO pins and other settings.


Usage
===============

 ## Start the daemon
 `/etc/init.d/terra start`
 or
 `terra daemon start` (use `terra daemon` for running out of OpenRC scope)

 ## Set socket manually
 `terra switch <a/b/c>(:<channel>) <on/off>` (example: `terra switch c off` or `terra switch a:24 on`)

 ## Schedule modes
 With modes, the behavior of schedules can be modified. Changing the mode will force the daemon to reload (SIGHUP) the config.
 ###### Read current schedule mode
 `sudo terra mode` will print the current mode to the terminal
 ###### Set new schedule mode
 `sudo terra mode set <mode>` will set given argument as new mode
 ###### Reset schedule mode
 `sudo terra mode reset` will reset the mode

 ## Hygrometer
 ###### Read hygrometer manually
 `terra hygro`
 ###### Show hygrometer recorded data
 `terra show`  
 ![alt text](https://github.com/sellep/terrap/blob/master/res/terra.show.png)
