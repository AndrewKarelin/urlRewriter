#ifndef __DICT_H_
#define __DICT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  /* table entry: */
{
	void *next; /* next entry in chain */
	char *name; /* key */
	char *defn; /* value */
} nlist;

/* lookup: look for key s in hashtab */
nlist *lookup(char *s, nlist **dict);

/* install: put (name, defn) in hashtab */
nlist *install(char *name, char *defn, nlist **dict);

/* initialization: create hashable with specified size */
nlist ** dict_init(int hashsize);

/* free memory allocated by dictionary */
void free_dict(nlist ** dict);

#ifdef __cplusplus
}
#endif

#endif /* __DICT_H_ */
