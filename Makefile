CC=@gcc
CFLAGS=-Wall -v -fomit-frame-pointer -pipe -O3 -DNCURSES `ncursesw5-config --cflags`
LIBS=-lrt -pthread `ncursesw5-config --libs`
DAEMON=-DSYSLOG_ENABLED

OBJ=pi_2_mmio.o \
	terra_lock.o \
	terra_proc.o \
	terra_time_print.o \
	terra_time_read.o \
	terra_time_diff.o \
	terra_time_sleep.o \
	terra_time_cmp.o \
	terra_date_now.o \
	terra_data_append.o \
	terra_data_read.o \
	terra_pin.o \
	terra_led_set.o \
	terra_led_arg.o \
	terra_led_set_from_cmd.o \
	terra_conf_read_global.o \
	terra_conf_read_sched.o \
	terra_conf_read_sched_periods.o \
	terra_conf_read_sched_clocks.o \
	terra_conf_read_sched_temps.o \
	terra_conf_read.o \
	terra_conf_valid.o \
	terra_conf_print.o \
	terra_switch_arg.o \
	terra_switch_get_code.o \
	terra_switch_transmit.o \
	terra_switch_set.o \
	terra_hygro_run.o \
	terra_hygro_read.o \
	terra_hygro_write.o \
	terra_visual_show.o \
	terra_visual_bounding.o \
	terra_visual_title.o \
	terra_visual_grid.o \
	terra_visual_labels.o \
	terra_heart_beat.o \
	terra_init.o \
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

all: clean $(OBJ)
	$(CC) $(CFLAGS) -o obj/terra_log.o -c src/utils/terra_log.c
	$(CC) $(CFLAGS) -o bin/terra src/terra.c $(addprefix obj/, $(OBJ)) obj/terra_log.o $(LIBS)
	rm -f obj/terra_log.o
	$(CC) $(CFLAGS) $(DAEMON) -o obj/terra_log.o -c src/utils/terra_log.c
	$(CC) $(CFLAGS) $(DAEMON) -o bin/terrad src/terrad.c $(addprefix obj/, $(OBJ)) obj/terra_log.o $(LIBS)

debug_flags:
	$(eval CFLAGS += "-DDEBUG")
	$(eval CFLAGS += "-g")

debug: debug_flags all

install:
	@cp bin/terra /usr/local/bin/
	@cp bin/terrad /usr/local/bin/
	@cp res/terra.conf /etc/default/terra
	@cp res/terrad.init.script /etc/init.d/terra
	@chmod +x /etc/init.d/terra
	@mkdir -p /var/opt/terra

uninstall:
	@rm -f /usr/local/terra
	@rm -f /usr/local/terrad
	@rm -f /etc/default/terra
	@rm -f /etc/init.d/terra
	@rm -rf /var/opt/terra

clean:
	@mkdir -p bin obj
	@rm -rf bin/*
	@rm -rf obj/*
