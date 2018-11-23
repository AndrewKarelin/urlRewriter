#include "tools.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


char * parse_args(int argc, char *argv[])
{

	for (int i = 0; i < argc; i++)
		syslog(LOG_DEBUG, "Got argv[%d] = %s",i,argv[i]);

	if (argc < 2)
	{
		printf("usage %s input.json\n", argv[0]);
		syslog(LOG_ERR, "Too few arguments. Exiting. usage %s input.json", argv[0]);
		exit(-1);
	}

	return argv[1];
}

char * read_file(char *filename, int *bytes_read)
{
	char * buffer = NULL;
	*bytes_read = 0;

	int input_fd = open(filename, O_RDONLY);
	if (input_fd == -1)
	{
		syslog(LOG_ERR,"open: %s",strerror(errno));
		return NULL;
	}

	long file_size = lseek(input_fd, 0, SEEK_END);
	if (file_size == -1)
	{
		syslog(LOG_ERR,"lseek: %s",strerror(errno));
		close(input_fd);
		return NULL;
	}

	buffer = malloc(file_size);
	if (buffer == NULL)
	{
		syslog(LOG_ERR,"malloc: %s",strerror(errno));
		close(input_fd);
		return NULL;
	}

	lseek(input_fd, 0, SEEK_SET);

	*bytes_read = read(input_fd, buffer, (int) file_size);
	if (*bytes_read != file_size)
	{
		syslog(LOG_ERR, "read only %d from %d: %s", *bytes_read,
				(int) file_size, strerror(errno));
		free(buffer);
		close(input_fd);
		return NULL;
	}
	close(input_fd);
	return buffer;
}

token_t * parse_json(char *buffer, int bytes_read, int *records)
{
	token_t * t = NULL;
	*records = 0;

	int num_tokens = 0;

	for (int i = 0; i < bytes_read; i++)
		if (buffer[i] == '"')
			num_tokens++;

	num_tokens = num_tokens / 4;

	t = (token_t *) calloc(num_tokens, sizeof(token_t));

	if (t == NULL)
	{
		syslog(LOG_ERR,"calloc:%s",strerror(errno));
		return NULL;
	}

	int r = 0; //index of t array
	int state = 0;

	for (int i = 0; i < bytes_read; i++)
		if (buffer[i] == '"')
			switch (state)
			{
			case 0:
				t[r].start_key = i + 1;
				state = 1;
				break;
			case 1:
				state = 2;
				buffer[i] = 0; // for strlen
				if (buffer[i + 1] != ':')
				    syslog(LOG_WARNING,"Error parsing json. Expect ':' got %c\n",
							buffer[i + 1]);
				break;
			case 2:
				t[r].start_val = i + 1;
				state = 3;
				break;
			case 3:
				state = 0;
				buffer[i] = 0; // for strlen
				if ((buffer[i + 1] != ',') && (buffer[i + 1] != '}'))
					syslog(LOG_WARNING,"Error parsing json. Expect ',' got %c\n",
							buffer[i + 1]);
				r++; // go to next record
				break;
			}

	if (num_tokens != r)
	{
		syslog(LOG_ERR,"parse_json error. Found %d from expected %d\n", r, num_tokens);
		free(t);
		return NULL;
	}
	*records = r;
	return t;
}

nlist ** json_to_dict(token_t * t, char * buffer, int records)
{

	nlist **dict = dict_init(records * 1.5); // population about 70%

	if (dict == NULL)
	{
		syslog(LOG_ERR,"dict_init: %s",strerror(errno));
		return NULL;
	}

	for (int i = 0; i < records; i++)
	{
		char *key = buffer + t[i].start_key;
		char *value = buffer + t[i].start_val;

		if (install(key, value, dict) == NULL)
		{
			syslog(LOG_ERR, "dict install %s : %s : %s", key, value,
					strerror(errno));
			free_dict(dict);
			return NULL;
		}

	}
	return dict;
}

nlist ** load_dict(char *filename)
{
	int bytes_read = 0;

	char * buffer = read_file(filename, &bytes_read);

	if (buffer == NULL)
	{
		syslog(LOG_ERR,"Read file %s error.", filename);
		exit(-1);
	}

	int records = 0;

	token_t *json = parse_json(buffer, bytes_read, &records);

	if (json == NULL)
	{
		syslog(LOG_ERR,"Parse json error.");
		free(buffer);
		exit(-1);
	}

	nlist ** dict = json_to_dict(json, buffer, records);

	free(buffer);
	free(json);

	if (dict == NULL)
	{
		syslog(LOG_ERR,"Error loading redirect list from %s\n", filename);
		exit(-1);
	}

	return dict;
}
