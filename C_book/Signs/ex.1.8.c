// ---Упражнение 1.8. ---

#include <stdio.h>

  /* подсчет строк, пробелов и табуляций входного потока */
  int main()
  {
      int c = 0;
      int nl = 0;
      int ns = 0;
      int nt = 0;

      while ((c = getchar()) != EOF)
          if (c == '\n')
              ++nl;
          else if (c == ' ')
              ++ns;
          else if (c == '\t')
              ++nt;
      printf("\nlines=%d, spaces=%d, tabs=%d\n", nl, ns, nt);
      return 0;
  }
