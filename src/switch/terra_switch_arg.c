#include "terra_switch.h"

#define ARG_MODE_SWITCH_ON "on"
#define ARG_MODE_SWITCH_OFF "off"

BOOL terra_switch_arg(terra_switch_req * const switch_req, int const argc, char const * const * const argv)
{
	if (argc != 4)
	{
		terra_log_error("insufficient switch argument count\n");
		return FALSE;
	}

	if (strlen(argv[2]) != 1)
	{
		terra_log_error("invalid socket argument length\n");
		return FALSE;
	}

	if (   argv[2][0] != 'a'
		&& argv[2][0] != 'A'
		&& argv[2][0] != 'b'
		&& argv[2][0] != 'B'
		&& argv[2][0] != 'c'
		&& argv[2][0] != 'C'
		)
	{
		terra_log_error("invalid socket\n");
		return FALSE;
	}

	if (argv[2][0] == 'A')
	{
		switch_req->socket = 'a';
	}
	else if (argv[2][0] == 'B')
	{
		switch_req->socket = 'b';
	}
	else if (argv[2][0] == 'C')
	{
		switch_req->socket = 'c';
	}
	else
	{
		switch_req->socket = argv[2][0];
	}

	if (strcmp(argv[3], ARG_MODE_SWITCH_ON) == 0)
	{
		switch_req->mode = SWITCH_ON;
		return TRUE;
	}

	if (strcmp(argv[3], ARG_MODE_SWITCH_OFF) == 0)
	{
		switch_req->mode = SWITCH_OFF;
		return TRUE;
	}

	terra_log_error("invalid switch set argument\n");
	return FALSE;
}