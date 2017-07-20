NCURSESW6 := $(shell which ncursesw6-config 2> /dev/null)
NCURSESW5 := $(shell which ncursesw5-config 2> /dev/null)

CC=@gcc
CFLAGS=-v -fomit-frame-pointer -pipe -O3
LIBS=-lrt -pthread `pkg-config --libs libconfig`

OBJ=pi_2_mmio.o \
	common_dht_read.o \
	pi_2_dht_read.o \
	terra_log.o \
	terra_lock_init.o \
	terra_time_print.o \
	terra_time_parse.o \
	terra_data_append.o \
	terra_data_read.o \
	terra_led_arg.o \
	terra_conf_read.o \
	terra_conf_print.o \
	terra_switch_arg.o \
	terra_switch_transmit.o \
	terra_hygro_read.o \
	terra_hygro_write.o \
	terra_visual_show.o \
	terra_visual_grid_init.o \
	terra_visual_draw_title.o \
	terra_visual_draw_grid.o \
	terra_visual_draw_labels.o \
	terra_runtime.o \
	terra_schedule_run_period.o \
	terra_schedule_run_clock.o \
	terra_schedule_run_temp.o \
	terra_schedule_dependency.o \
	terra_schedule_run.o \
	terra_signal.o

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

%.o : src/schedule/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/visual/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

all: clean ncursesd_flags $(OBJ)
	$(CC) $(CFLAGS) -o bin/terra src/terra.c $(addprefix obj/, $(OBJ)) $(LIBS)

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
	@mkdir -p /etc/terra
	@cp res/terra.conf /etc/terra/terra.conf
	@cp res/terra.init.d /etc/init.d/terra
	@chmod +x /etc/init.d/terra
	@mkdir -p /var/lib/terra

uninstall:
	@rm -f /opt/terra
	@rm -rf /etc/terra
	@rm -f /etc/init.d/terra
	@rm -rf /var/lib/terra

clean:
	@mkdir -p bin obj
	@rm -rf bin/*
	@rm -rf obj/*

.PHONY: clean uninstall install debug debug_flags all ncursesd_flags
