#include "config.h"
#include "main.h"
#include "echo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

int echo_entry(config_opt config_opts[])
{
	int i, port;
	char proto[5];

	for(i=0;;i++)
	{
		if (!strcmp(config_opts[i].name, ""))
		{
			// End of options
			break;
		}
		if (!strcmp(config_opts[i].name, "port"))
		{
			port = atoi(config_opts[i].value);
			if (port <= 0 || port >= 65535)
			{
				fprintf(stderr, "Invalid port specification: %s\n", config_opts[i].value);
				return 1;
			}
		}
		else if (!strcmp(config_opts[i].name, "proto"))
		{
			strncpy(proto, config_opts[i].value, sizeof(proto));
			proto[sizeof(proto)-1] = 0;
			if (strcmp(proto, "tcp") && strcmp(proto, "udp"))
			{
				fprintf(stderr, "Invalid proto specification '%s'. Must be one of: tcp, udp\n", config_opts[i].value);
				return 1;
			}
		}
		else
		{
			fprintf(stderr, "Unsupported option: %s\n", config_opts[i].name);
			return 1;
		}
	}

	server_info *srv_info = (server_info *)calloc(1, sizeof(server_info));
	strcpy(srv_info->proto, proto);
	srv_info->port = port;
	srv_info->recv_ready_callback = echo_recv_ready;

	i = server_start(srv_info);

	free(srv_info);

	return 0;
}

int echo_recv_ready(Socket *sock)
{
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	char buf[1024];

	while (1)
	{
		if (sock->recvready(sock, 0))
		{
			sock->recvfrom(sock, buf, sizeof(buf), (struct sockaddr *) &client_addr, &len);
			printf("echo_recv_ready(): received string '%s'\n", buf);
			sock->sendto(sock, buf, (struct sockaddr *) &client_addr, len);
		}
	}

}
