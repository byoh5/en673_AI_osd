/*
	@base 		: start of data in memory 
	@nel  		: number of elements to be sorted
	@width		: size (in bytes) of each element
	@compare	: comparison function 
*/
void
qsort(char base[], int nel, int width, int (*compar)(const void *a, const void *b));