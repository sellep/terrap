#include "terra_switch.h"

#define SOCKET_RESULT(c) *socket = (c); return TRUE;
#define MODE_RESULT(m) *mode = (m); return TRUE;

static inline BOOL switch_socket(char * const socket, char const * const arg)
{
	if (arg[0] == 'a' || arg[0] == 'A')
	{
		SOCKET_RESULT('a');
	}

	if (arg[0] == 'b' || arg[0] == 'B')
	{
		SOCKET_RESULT('b');
	}

	if (arg[0] == 'c' || arg[0] == 'C')
	{
		SOCKET_RESULT('c');
	}
	
	if (arg[0] == 'd' || arg[0] == 'D')
	{
		SOCKET_RESULT('d');
	}

	return FALSE;
}

static inline BOOL switch_channel(int * const channel, char const * const arg)
{
	if (strlen(arg) == 1)
		return TRUE;

	if (strlen(arg) == 2)
		return FALSE;

	if (arg[1] != ':')
		return FALSE;

	*channel = atoi(arg[2]);

	if (channel == 0)
		return FALSE;

	return TRUE;
}

static inline BOOL switch_mode(terra_switch_mode * const mode, char const * const arg)
{
	if (strcmp(arg, "on"))
	{
		MODE_RESULT(SWITCH_ON);
	}

	if (strcmp(arg, "ON"))
	{
		MODE_RESULT(SWITCH_ON);
	}

	if (strcmp(arg, "off"))
	{
		MODE_RESULT(SWITCH_OFF);
	}

	if (strcmp(arg, "OFF"))
	{
		MODE_RESULT(SWITCH_OFF);
	}

	return FALSE;
}

BOOL terra_switch_run(int const argc, char * * const argv, terra_switch_mode const * const modes, terra_conf_switch const * const conf)
{
	char socket;
	int channel;
	terra_switch_mode mode;

	if (argc == 2)
	{
		terra_switch_print(modes, conf);
	}
	else if (argc == 4)
	{
		if (!switch_socket(&socket, argv[2]))
		{
			terra_log_error("[terra_switch_run] failed to parse switch socket\n");
			return FALSE;
		}

		if (!switch_channel(&channel, argv[2]))
		{
			terra_log_error("[terra_switch_run] failed to parse switch channel\n");
			return FALSE;
		}

		if (!switch_mode(&mode, argv[3]))
		{
			terra_log_error("[terra_switch_run] failed to parse switch mode\n");
			return FALSE;
		}

		terra_switch_set(conf, socket, channel, mode, (char*) NULL);
	}
	else
	{
		terra_log_error("[terra_switch_run] insufficient arguments\n");
		return FALSE;
	}

	return TRUE;
}