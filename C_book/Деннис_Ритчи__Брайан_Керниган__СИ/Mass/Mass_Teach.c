#include <stdio.h>

int main()
{
	int c = 0;
	int i;
	int nwhite = 0 ;
	int nother = 0;
	int ndigit[10];
	nwhite = nother = 0;
	for (i = 0; i < 10; ++i)
		ndigit[i]= 0;
	while ((c = getchar()) != EOF)
	if (c >= '0' && c <= '9' )
		++ndigit[c - '0'];
	else if (c == ' ' || c == '\n' || c == '\t')
		++nwhite;
	else
		++nother;
	printf ("цифры =");
	for (i=0; i < 10; ++i)
		printf(" %d", ndigit[i]);
	printf (", символы-разделители = %d, прочие = %d\n", nwhite, nother);
}
