
all: clean
	gcc -o obj/common_dht_read.o -c common_dht_read.c
	gcc -o obj/pi_2_mmio.o -c pi_2_mmio.c
	gcc -o obj/pi_2_dht_read.o -c pi_2_dht_read.c
	gcc -o bin/a.out main.c obj/common_dht_read.o obj/pi_2_mmio.o obj/pi_2_dht_read.o

clean:
	mkdir -p obj bin
	rm -rf obj/*
	rm -rf bin/*
