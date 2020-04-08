#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <bsd/string.h>

#include "slist.h"
#include "pages.h"
#include "inode.h"

typedef struct dircurrent {
	const char* name;
	int inum;
} dircurrent; 

char* directory_get(int inum);
char* directory_current(const char* path); 
void directory_init();
int directory_lookup_empty(); 
int directory_lookup(const char* name);
int tree_lookup(const char* path);
int directory_put(const char* name, int inum);
int directory_delete(const char* name);
slist* directory_list(const char* path);
void print_directory();

#endif

