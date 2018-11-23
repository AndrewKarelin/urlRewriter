#ifndef __TOOLS_H_
#define __TOOLS_H_

#include "dict.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int start_key;
	int start_val;
} token_t;

/* parse command line arguments and return name of json file */
char * parse_args(int argc, char *argv[]);

/* read data from file to dictoinary*/
nlist ** load_dict(char *filename);

/* read file from command line argument to memory*/
char * read_file(char *filename, int *bytes_read);

/* parse string to token array*/
token_t * parse_json(char *buffer, int bytes_read, int *);

/* put data to dictionary*/
nlist ** json_to_dict(token_t * t, char * buffer, int records);

#ifdef __cplusplus
}
#endif

#endif /* __TOOLS_H_ */
