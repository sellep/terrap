#ifndef __P_TERRA_LOG_READER_H
#define __P_TERRA_LOG_READER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	ssize_t hour;
	ssize_t min;
	ssize_t sec;
} terra_time;

typedef struct
{
	terra_time tm;
	float humi;
	float temp;
} terra_log_entry;

int terra_log_read_entry(terra_log_entry * const entry, char const * const line)
{
	ssize_t i;

	entry->tm.hour = atoi(line);

	if (line[2] != ':')
		return -1;

	entry->tm.min = atoi(line + 3);

	if (line[5] != ':')
		return -2;

	entry->tm.min = atoi(line + 6);

	if (line[8] != ' ')
		return -3;

	entry->humi = atof(line + 9);

	i = 9;
	while (1)
	{
		if (line[i] == ' ')
			break;

		if (line[i] == '\n')
			return -4;

		if (line[i] == '\0')
			return -5;

		i++;
	}

	if (line[i] != ' ')
		return -6;

	entry->temp = atof(line + i + 1);

	return 0;
}

int terra_log_read(terra_log_entry * * const entries, char const * const path)
{
	FILE *f;
	char *line = NULL;
	size_t buf_len = 0;
	int read;
	size_t result = 0;
	size_t i;

	f = fopen(path, "r");
	if (!f)
		return -1;

	//get line count

	while ((read = getline(&line, &buf_len, f)) > 0)
	{
		result++;
	}

	entries[0] = (terra_log_entry*) malloc(sizeof(terra_log_entry) * result);

	//read lines	

	rewind(f);
	
	i = 0;

	while ((read = getline(&line, &buf_len, f)) > 0)
	{
		if (terra_log_read_entry(&entries[0][i], line) < 0)
		{
			fprintf(stderr, "failed to read line %zu\n", i);
			result = -1;
			break;
		}

		i++;
	}

exit:
	fclose(f);
	free(line);
	return result;
}

#endif
