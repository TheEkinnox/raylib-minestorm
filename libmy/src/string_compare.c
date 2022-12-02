#include <stdlib.h>
#include "string_compare.h"
#include "string_length.h"

int string_compare(char const* str1, char const* str2)
{
	if (str1 == NULL && str2 == NULL)
		return 0;
	else if (str1 == NULL)
		return -1;
	else if (str2 == NULL)
		return 1;

	while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2)
	{
		str1++;
		str2++;
	}

	if (*str1 == *str2)
	{
		int len1 = string_length(str1);
		int len2 = string_length(str2);

		if (len1 == len2)
			return 0;

		return len1 > len2 ? 1 : -1;
	}

	return *str1 > *str2 ? 1 : -1;
}