NCURSESW6 := $(shell which ncursesw6-config 2> /dev/null)
NCURSESW5 := $(shell which ncursesw5-config 2> /dev/null)

CC=@gcc
CFLAGS=-v -fomit-frame-pointer -pipe -O3
LIBS=-lrt -pthread
SYSLOG=-DSYSLOG_ENABLED

OBJ=pi_2_mmio.o \
	common_dht_read.o \
	pi_2_dht_read.o \
	terra_time_print.o \
	terra_time_read.o \
	terra_data_append.o \
	terra_data_read.o \
	terra_led_arg.o \
	terra_conf_read.o \
	terra_conf_print.o \
	terra_switch_arg.o \
	terra_switch_set.o \
	terra_hygro_run.o \
	terra_hygro_write.o \
	terra_visual_show.o \
	terra_visual_bounding.o \
	terra_visual_title.o \
	terra_visual_grid.o \
	terra_visual_labels.o \
	terra_visual_legend.o \
	terra_visual_points.o \
	terra_runtime.o \
	terrad_run_period.o \
	terrad_run_clock.o \
	terrad_run_temp.o \
	terrad_run.o

%.o : src/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/conf/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/led/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/switch/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/hygro/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/utils/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/visual/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

all: clean ncursesd_flags $(OBJ)
	$(CC) $(CFLAGS) -o obj/terra_log.o -c src/utils/terra_log.c
	$(CC) $(CFLAGS) -o bin/terra src/terra.c $(addprefix obj/, $(OBJ)) obj/terra_log.o $(LIBS)
	rm -f obj/terra_log.o
	$(CC) $(CFLAGS) $(SYSLOG) -o obj/terra_log.o -c src/utils/terra_log.c
	$(CC) $(CFLAGS) $(SYSLOG) -o bin/terrad src/terrad.c $(addprefix obj/, $(OBJ)) obj/terra_log.o $(LIBS)

ncursesd_flags:
ifneq (, $(NCURSESW6))
	$(eval CFLAGS += "-DNCURSES")
	$(eval CFLAGS += `ncursesw6-config --cflags`)
	$(eval LIBS += `ncursesw6-config --libs`)
else ifneq (, $(NCURSESW5))
	$(eval CFLAGS += "-DNCURSES")
	$(eval CFLAGS += `ncursesw5-config --cflags`)
	$(eval LIBS += `ncursesw5-config --libs`)
else
	@echo no ncursesw
endif

debug_flags:
	$(eval CFLAGS += "-DDEBUG")
	$(eval CFLAGS += "-g")

debug: debug_flags all

install:
	@cp bin/terra /usr/local/bin/
	@cp bin/terrad /usr/local/bin/
	@cp res/terra.conf /etc/conf.d/terra
	@cp res/terrad.init.script /etc/init.d/terra
	@chmod +x /etc/init.d/terra
	@mkdir -p /var/opt/terra

uninstall:
	@rm -f /opt/terra
	@rm -f /opt/terrad
	@rm -f /etc/conf.d/terra
	@rm -f /etc/init.d/terra
	@rm -rf /var/opt/terra

clean:
	@mkdir -p bin obj
	@rm -rf bin/*
	@rm -rf obj/*

.PHONY: clean uninstall install debug debug_flags all ncursesd_flags
