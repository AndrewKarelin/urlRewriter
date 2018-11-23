#include "dict.h"

#include <stdlib.h>
#include <string.h>
#include <syslog.h>

volatile static int hashsize;

/* initialization: create dictionary with specified size */
nlist ** dict_init(int newhashsize)
{
	hashsize = newhashsize;

	syslog(LOG_DEBUG, "Create new dictionary size %d", hashsize);

	return (nlist **) calloc(newhashsize, sizeof(nlist *));
}

/* hash: form hash value for string s */
unsigned hash(char *s)
{
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % hashsize;
}

/* lookup: look for s in dictionary */
nlist *lookup(char *s, nlist **dict)
{
	nlist *np;
	for (np = dict[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np; /* found */
	return NULL; /* not found */
}

/* make a duplicate of s */
char *mystrdup(char *s)
{
	char *p;
	p = (char *) malloc(strlen(s) + 1); /* +1 for ’\0’ */
	if (p != NULL)
		strcpy(p, s);
	return p;
}

/* install: put (name, defn) in hashtab */
nlist *install(char *name, char *defn, nlist **dict)
{
	nlist *np;
	unsigned hashval;

	if ((np = lookup(name, dict)) == NULL) // not found
	{
		np = (nlist *) calloc(1, sizeof(*np));
		if (np == NULL || (np->name = mystrdup(name)) == NULL)
			return NULL;

		hashval = hash(name);
		np->next = dict[hashval];
		dict[hashval] = np;
	}
	else
		//	already there - replace
		free((void *) np->defn); /*free previous value */

	if ((np->defn = mystrdup(defn)) == NULL)
		return NULL;

	return np;
}

void free_dict(nlist ** dict)
{
	nlist *np, *next;

	syslog(LOG_DEBUG, "Free dictionary");


	for (int i = 0; i < hashsize; i++)
	{
		np = dict[i];

		while (np != NULL)
		{
			next = np->next;
			free(np->defn);
			free(np->name);
			free(np);
			np = next;
		}

	}
	free(dict);
	hashsize = 0;
}

