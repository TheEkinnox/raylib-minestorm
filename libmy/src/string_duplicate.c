#include <stdlib.h>
#include "string_duplicate.h"
#include "string_length.h"
#include "string_sized_copy.h"

char* string_duplicate(char const* str)
{
	if (str == NULL)
		return NULL;

	unsigned int	strLen = string_length(str);
	char*			copy = malloc(strLen + 1 * sizeof(char));

	if (copy == NULL)
		return NULL;

	return string_sized_copy(copy, str, strLen + 1);
}