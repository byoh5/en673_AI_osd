#include "dev.h"
#include "dev_inline.h"

#include "share_rtspoverhttp.h"

void lookForHeader(char const* headerName, char const* source, unsigned int sourceLen, char* resultStr, unsigned int resultMaxSize)
{
	unsigned int i, j;
	resultStr[0] = '\0';  // by default, return an empty string
	unsigned int headerNameLen = strlen(headerName);

	for (i=0; i<(sourceLen-headerNameLen); ++i)
	{
		if (strncmp(&source[i], headerName, headerNameLen) == 0 && source[i+headerNameLen] == ':')
		{
			// We found the header.  Skip over any whitespace, then copy the rest of the line to "resultStr":
			for (i+=headerNameLen+1; (i<sourceLen)&&((source[i]==' ')||(source[i]=='\t')); ++i);	// for loop search!

			for (j=i; j<sourceLen; ++j)
			{
				if (source[j] == '\r' || source[j] == '\n')
				{
					// We've found the end of the line.  Copy it to the result (if it will fit):
					if (j-i+1 > resultMaxSize)
						break;

					char const* resultSource = &source[i];
					char const* resultSourceEnd = &source[j];
					while (resultSource < resultSourceEnd)
						*resultStr++ = *resultSource++;
					*resultStr = '\0';
					break;
				}
			}
		}
	}
}
