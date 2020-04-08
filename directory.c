
#define _GNU_SOURCE
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#include "directory.h"
#include "pages.h"
#include "slist.h"
#include "util.h"
#include "inode.h"

#define ENT_SIZE 16

static struct dircurrent* curr; 

void
directory_init()
{
    inode* rn = get_inode(1);

    if (rn->mode == 0) {
        rn->size = 0;
        rn->mode = 040755;
    }
}

char*
directory_current(const char* path) {
	curr->name = path;
	curr->inum = tree_lookup(path); 
}

char*
directory_get(int ii)
{
    char* base = pages_get_page(1);
    return base + ii*ENT_SIZE;
}

int
directory_lookup_empty() {
	printf("CALCULATING EMPTY\n"); 
	for (int ii = 0; ii < 256; ii++) {
		char* ent = directory_get(ii);
		if (ent == NULL) {
			printf("ENTRT IS %s\n", ent); 
		       return ii;
		}
	}

	return -ENOENT; 
}	

int
directory_lookup(const char* name)
{
    for (int ii = 0; ii < 256; ++ii) {
        char* ent = directory_get(ii);
        if (streq(ent, name)) {
            return ii;
        }
    }
    return -ENOENT;
}

int
tree_lookup(const char* path)
{
    assert(path[0] == '/');

    if (streq(path, "/")) {
        return 1;
    }

    return directory_lookup(path + 1);
}

int
directory_put(const char* name, int inum)
{
    char* ent = pages_get_page(1) + inum*ENT_SIZE;
    strlcpy(ent, name, 48);
    printf("+ dirent = '%s'\n", ent);

    printf("THE NAME IS %s\n", ent); 

    inode* node = get_inode(inum);
    printf("+ directory_put(..., %s, %d) -> 0\n", name, inum);
    print_inode(node);

    return 0;
}

int
directory_delete(const char* name)
{
    printf(" + directory_delete(%s)\n", name);

    int inum = directory_lookup(name);
	    free_inode(inum);
    	    char* ent = pages_get_page(1) + inum*ENT_SIZE;
    	    ent[0] = 0;

    return 0;
}

slist*
directory_list(const char* path)
{
    printf("+ directory_list()\n");
    //printf("WE ARE PRINTING DIRECTORY\n"); 
    slist* ys = 0;

    const char* real_path = path + 1;

    //printf("Path is %s\n", real_path);

    if (strstr(path, "test2")) {
	   printf("strstr test worked\n");
    } 

    for (int ii = 0; ii < 256; ++ii) {
        char* ent = directory_get(ii);
        if (ent[0]) {
	    //printf("THE ENTRY IS %s\n", ent);
	    if (streq(path, "/")) {
		    //printf("WE ARE IN ROOT\n"); 
		    //anything that doesn't have a slash
		    //if (strstr(ent, "/") == 0) {
			    printf(" - %d: %s [%d]\n", ii, ent, ii);
			    ys = s_cons(ent, ys);
		    //}
	    }

	    else {
		    //printf("WE ARE IN DIRECTORY %s\n", real_path); 
		    if (strstr(ent, "/") == 0) {
			    //doesn't contain a slash, definitely in root so not here
			    printf("This file doesn't belong here: %s\n", ent);
		    }

		    else if (strstr(ent, real_path)) {
			    //we've found it, need from path on
			    printf("We've found file: %s\n", ent);
			    char* startPath = strstr(ent, real_path);
			    printf("Start path is %s\n", startPath); 

			    if (strstr(startPath, "/")) {
				printf("WE HAVE A SLASH\n"); 
			    	char* nextPath = strstr(startPath, "/");
			        char* experPath = nextPath + 1; 	
			    	printf("Starting point is %s\n", experPath);
			        ys = s_cons(experPath, ys); 	
			    }


			    //ys = s_cons(ent, ys);  
		    }

		    else if (strstr(ent, real_path) == 0) {
			    //printf("Path is %s, ent is %s\n", real_path, ent); 
		    }


	    }

        }
    }

    return ys;
}

void
print_directory(inode* dd)
{
    printf("Contents:\n");
    /*slist* items = directory_list(dd);
    for (slist* xs = items; xs != 0; xs = xs->next) {
        printf("- %s\n", xs->data);
    }*/
    printf("(end of contents)\n");
    //s_free(items);
}
