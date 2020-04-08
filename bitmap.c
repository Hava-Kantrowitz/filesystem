#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bitmap.h"

int bitmap_get(void* bm, int ii) {
	//get the ith element of bitmap bm
	int* result = (int*)bm + ii;
        int answer = *result; 	
	return answer; 
}

void bitmap_put(void* bm, int ii, int vv) {
	//put item vv in iith slot of bm
	int* addr = (int*)bm + ii;
	*addr = vv; 
}

void bitmap_print(void* bm, int size) {
	//loop through bm of size size, printing each element
	int* map = (int*)bm; 
	for (int i = 0; i < size; i++) {
		printf("%d", map[i]);
	}

	printf("\n"); 
}
