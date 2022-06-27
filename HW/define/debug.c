#include <stdio.h>

#define debug_print(fmt,...)\
				do{if(DEBUG) printf(stderr, "%s:%d:%s()" fmt, __FILE__,}\
					__LINE__, __func__, __VA_ARGS__);}while(0)


int main(void)
{
// ...
	debug_print("a != 13\n", a);
// ...
}
