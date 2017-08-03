```sh
go get github.com/labstack/gommon/color
```

terra
===============

 terra is a service to control electrical (radio) outlets, based on clock, periodic or hygrometer data scheduling


Prerequisites
===============

 terra is designed to run on Raspberry Pi 2/3 and Gentoo GNU/Linux.

 terra depends on  
 * OpenRC  
 * ncursesw 5+ (wide character support, optional)  
 * libconfig  


Installation
===============

 `git clone https://github.com/sellep/terrap.git`  
 `cd terrap`  
 `make`  
 `sudo make install`  

 `make install` will copy the default configuration file *res/terra.conf* to */etc/terra/terra.conf*, the init script *res/terra.init.d* to */etc/init.d/terra* and the binary to */usr/local/bin/terra*.

 ```go
 color.Println(color.Yellow("IMPORTANT: Please check the configuration file, you might want to modify the BCM GPIO pins and other settings like schedules"))
 ```


Usage
===============

 ## Daemon
 There are several ways to start the terra daemon.
 #### Synchronous
 It is recommended to use the asynchronous approach.  
 `sudo terra schedule`
 ###### Parameter
 * `logfile` will print all log messages to the log file */var/log/terra*.
 * `pidfile` will use and lock */var/run/terra.pid*.  This allows to reload the configuration from the outside.
 #### Asynchronous
 `sudo terra daemon` will fork and exec `terra schedule logfile pidfile`
 #### OpenRC
 Requires Gentoos Helper Function (https://dev.gentoo.org/~zmedico/portage/doc/ch06s02.html)
 `sudo /etc/init.d/terra start`

 ## Set socket manually
 `terra switch <socket> <on/off>` to set a given socket on or off  
 `terra switch <socket:channel> <on/off>` to set a given socket of a given channel on or off

 ## Schedule modes
 With modes, the behavior of schedules can be modified. Changing the mode will force the daemon to reload (SIGHUP) the config.
 #### Read current schedule mode
 `sudo terra mode` will print the current mode to the terminal
 #### Set new schedule mode
 `sudo terra mode set <mode>` will set given argument as new mode
 #### Reset schedule mode
 `sudo terra mode reset` will reset the mode

 ## Hygrometer
 #### Read hygrometer manually
 `terra hygro`
 #### Show hygrometer recorded data
 `terra show`  
 `terra show <days_in_past>` to show past data  
 `terra show <days_in_past> <start_time>` to show past data starting at a given time (`terra show 0 15:00`)  
  Example:  
 ![alt text](https://github.com/sellep/terrap/blob/master/res/terra.show.png)
