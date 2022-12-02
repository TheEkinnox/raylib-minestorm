#include <stdlib.h>
#include "string_sized_copy.h"
#include "string_length.h"

char* string_sized_copy(char* dest, char const* src, unsigned int size_max)
{
	if (src == NULL || dest == NULL)
		return (dest = NULL);

	unsigned int srcLen = string_length(src) + 1;
	unsigned int targetSize = size_max < srcLen ? size_max : srcLen;

	for (unsigned int i = 0; i < targetSize; i++)
		dest[i] = src[i];

	return dest;
}