#include <stdio.h>
#include <math.h>


#define STEP 2 * 60
#define BEGIN 5 * 60 * 60 + 15 * 60
#define END 23 * 60 * 60 + 15 * 60
#define LEN (END - BEGIN)
#define HLEN (LEN/2.0)

#define TEMP_MIN 23
#define TEMP_RISE ((double)8)
#define TEMP_HRISE (TEMP_RISE/2.0)

typedef struct
{
	ssize_t hour;
	ssize_t min;
	ssize_t sec;
} terra_time;

static inline void itott(terra_time * const tt, size_t const i)
{
	size_t val = i;

	tt->hour = val / 60 / 60;
	val -= tt->hour * 60 * 60;
	tt->min = val / 60;
	val -= tt->min * 60;
	tt->sec = val;
}

int main()
{
	FILE *f;
	terra_time tt;
	size_t time;
	size_t x;
	float humi;
	float temp;

	f = fopen("test_data", "w");

	for (time = BEGIN; time <= END; time += STEP, x += STEP)
	{
		itott(&tt, time);
		temp = (float)(-cos(M_PI * x / HLEN) * TEMP_HRISE + TEMP_HRISE + TEMP_MIN);
		humi = TEMP_MIN;

		fprintf(f, "%02zu:%02zu:%02zu %.2f %.2f\n", tt.hour, tt.min, tt.sec, humi, temp);
	}

	fclose(f);
}
