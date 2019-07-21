#define BASE(i) &base[(i)*width]

/*
	@base 		: start of data in memory 
	@nel  		: number of elements to be sorted
	@width		: size (in bytes) of each element
	@compare	: comparison function 
*/
void
qsort(char base[], int nel, int width, int (*compar)(const void *a, const void *b))
{
    int gap;
    int i;
    int j;

    register int k;	/* inner exchange loop parameters */
    register char* p;
    register char* q;
    register char  c;

    for (gap = nel/2; gap > 0; gap /= 2)
    {
		for (i = gap; i < nel; i++)
		{
	    	for (j = i-gap; j >= 0; j -= gap)
	    	{
	        	p = BASE(j);
				q = BASE(j+gap);
				if ((*compar)(p,q) <= 0)
			    	break;	/* exit j loop */
				else
				{
			    	for (k = 0; k < width; (++p, ++q, ++k))
			    	{
						c = *q;
						*q = *p;
						*p = c;
			    	}
				}
	    	}
		}
    }
}