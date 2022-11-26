#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>

#include <limits.h>
#include <sched.h>
#include <string.h>


// in *nix I trust (if /proc/<PID>/stat exists it's format is valid)
static void
get_proc_stat_name (FILE *fs, char *cbuf)
{
  int c;

  while ((c = fgetc(fs)) != '(');

  while ((c = fgetc(fs)) != ')')
    *cbuf++ = c;
  *cbuf = 0;

}

// returns 0, if exec() in `pid` process failed
static int
check_exec (pid_t pid, struct stat *parent, const char *p_name)
{
  struct stat st;

  char cbuf[PATH_MAX]; /* yes, I’m being cheap ("жаба душит") -) */
  snprintf(cbuf, PATH_MAX, "/proc/%d/exe", (int)pid);

  do {
    if (stat(cbuf, &st)) { // now zombie (realy, the /proc/<PID>/exe is link to not existing file)
      snprintf(cbuf, PATH_MAX, "/proc/%d/stat", (int)pid);
      FILE *fs = fopen(cbuf, "r");
      if (!fs)
        exit((perror(cbuf), -1));

      get_proc_stat_name(fs, cbuf);
      fclose(fs);

      if (strcmp(p_name, cbuf) == 0)
        return 1; // exec() was successful and command already completed
      break;
    }
    if (parent->st_ino != st.st_ino || parent->st_dev != st.st_dev) // change, so exec() OK
      return 1;

    // exec() still not called
  } while (sched_yield() == 0);

  return 0;
}

int
main (int ac, char *av[])
{
  if (ac < 3)
    exit((puts("Usage: ./a.out p1 p2 ..."), 1));

  pid_t pid;
  int pfd[2], input;
  struct stat st;
  stat("/proc/self/exe", &st);

  // этот цикл, собственно, решение задачи в вопросе -- запуск конвейера команд 
  for (int i = 1; i < ac; i++) {
    pipe(pfd);

    if (!(pid = fork())) {
      //      printf("child %d of %d grp %d\n", (int)getpid(), (int)getppid(), (int)getpgrp());

      if (i != 1) {      // first command read inhereted stdin
        dup2(input, 0);  //   any other read pfd[0] from previous iteration
        close(input);
      }
      if (i != ac - 1)   // last command write to inhereted stdout
        dup2(pfd[1], 1); //   any other write to current pfd[1]
      close(pfd[0]);
      close(pfd[1]);

      char *argv [2] = {av[i], NULL};
      execvp(av[i], argv);
      fprintf(stderr, "command `%s` not found, terminate\n", av[i]);
      exit(-1);
    }

    //    printf("fork to %d\n", (int)pid);
    //puts(">>>"); getchar();

    if (i != 1)
      close(input);
    close(pfd[1]);
    input = pfd[0];  // the next member of commands pipe will read this fd

    // Well,  the icing on the cake
    if (!check_exec(pid, &st, av[i]))
      killpg(getpgrp(), SIGTERM);
  }

  close(input);

  while ((pid = wait(0)) != -1);
    //    printf("fin: %d\n", pid);

  return puts("End") == EOF;
} 