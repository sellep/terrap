#include "terra_data.h"

static char _buf[50];

inline static size_t get_line_count(FILE * const f, char * * const line, size_t * const buf_len)
{
	size_t lines = 0;
	while (getline(line, buf_len, f) > 0) lines++;
	return lines;
}

static BOOL terra_data_read_entry(terra_data_entry * const entry, char const * const line)
{
	ssize_t i;

	entry->tm.hour = atoi(line);

	if (line[2] != ':')
		return FALSE;

	entry->tm.min = atoi(line + 3);

	if (line[5] != ':')
		return FALSE;

	entry->tm.min = atoi(line + 6);

	if (line[8] != ' ')
		return FALSE;

	entry->humi = atof(line + 9);

	i = 9;
	while (1)
	{
		if (line[i] == ' ')
			break;

		if (line[i] == '\n')
			return FALSE;

		if (line[i] == '\0')
			return FALSE;

		i++;
	}

	if (line[i] != ' ')
		return FALSE;

	entry->temp = atof(line + i + 1);

	return TRUE;
}

BOOL terra_data_read(terra_data_entry * * const entries, size_t * const lines, ssize_t const back)
{
	terra_date date;
	FILE *f;
	BOOL result = FALSE;

	char *line = NULL;
	size_t buf_len;
	int read;
	size_t i;

	terra_date_now(&date, -back);
	sprintf(_buf, DATA_PATH, date.day, date.mon, date.year);

	f = fopen(_buf, "r");
	if (!f)
	{
		terra_log_error("[terra_data_read] failed to open file %s\n", _buf);
		return FALSE;
	}

	lines[0] = get_line_count(f, &line, &buf_len);
	if (lines[0] == 0)
	{
		terra_log_error("[terra_data_read] failed to read file %s\n", _buf);
		goto error;
	}

	entries[0] = (terra_data_entry*) malloc(sizeof(terra_data_entry) * lines[0]);
	rewind(f);

	i = 0;
	while ((read = getline(&line, &buf_len, f)) > 0)
	{
		if (!terra_data_read_entry(&entries[0][i], line))
		{
			terra_log_error("[terra_data_read] failed to read line %zu of file %s\n", i, _buf);
			goto error;
		}
	}

	result = TRUE;

error:
	fclose(f);
	free(line);
	return result;
}