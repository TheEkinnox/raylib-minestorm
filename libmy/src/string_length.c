#include <stdlib.h>
#include "string_length.h"

unsigned int string_length(char const* str)
{
	if (str == NULL)
		return 0;

	unsigned int count = 0;

	while (*str != '\0')
	{
		str++;
		count++;
	}

	return count;
}