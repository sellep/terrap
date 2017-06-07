CC=@gcc
CFLAGS=-Wall -v -march=haswell -fomit-frame-pointer -O3 -pipe
DWPI=-DWPI_ENABLED

OBJ=terra_time_print.o \
	terra_time_read.o \
	terra_conf_read_global.o \
	terra_conf_read_sched.o \
	terra_conf_read_sched_clocks.o \
	terra_conf_read.o \
	terra_conf_print.o \
	terra_switch_set_pin.o \
	terra_switch_arg.o \
	terra_switch_get_sock_code.o \
	terra_switch_transmit.o \
	terra_switch_set.o

%.o : src/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/conf/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/switch/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

%.o : src/utils/%.c
	$(CC) $(CFLAGS) -o obj/$@ -c $<

all: clean $(OBJ)
	$(CC) $(CFLAGS) -o bin/terra src/terra.c $(addprefix obj/, $(OBJ))

wpi:
	$(eval CFLAGS += "$(DWPI)")

arm: wpi all

clean:
	@mkdir -p bin obj
	@rm -rf bin/*
	@rm -rf obj/*
