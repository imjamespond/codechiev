/*
  The following program creates a pipe, and then fork(2)s to create
  a child process; the child inherits a duplicate set of file
  descriptors that refer to the same pipe.  After the fork(2), each
  process closes the file descriptors that it doesn't need for the
  pipe (see pipe(7)).  The parent then writes the string contained
  in the program's command-line argument to the pipe, and the child
  reads this string a byte at a time from the pipe and echoes it on
  standard output.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // memset,
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
  int pipefd[2];
  pid_t cpid;
  char buf[2]; 

  // if (argc != 2)
  // {
  //   fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  //   exit(EXIT_FAILURE);
  // }

  if (pipe(pipefd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  cpid = fork();
  if (cpid == -1)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0)
  {                   /* Child reads from pipe */
    close(pipefd[1]); /* Close unused write end */
 
    while (read(pipefd[0], buf, sizeof(buf)) > 0)
    {
      write(STDOUT_FILENO, buf, sizeof(buf)); // fork 不能printf?
      write(STDOUT_FILENO, ",", 1);
      ::memset(buf, 0, sizeof(buf));
    }
    write(STDOUT_FILENO, "\n", 1);
    close(pipefd[0]);
    _exit(EXIT_SUCCESS);
  }
  else
  {                   /* Parent writes argv[1] to pipe */
    close(pipefd[0]); /* Close unused read end */
    // write(pipefd[1], argv[1], strlen(argv[1]));
    std::string input;
    std::cin >> input;
    write(pipefd[1], input.c_str(), input.length());
    close(pipefd[1]); /* Reader will see EOF */
    wait(NULL);       /* Wait for child */
    exit(EXIT_SUCCESS);
  }
}
