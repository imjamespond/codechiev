#include <stdio.h>  // perror
#include <stdlib.h> // exit 
#include <signal.h> // kill
#include <sys/types.h> // SIGTERAM
#include <string>
#include <iostream>
#include <string.h> // memset,

#include <epoll/pipe.hpp> 

learn_cpp::Pipe pp;

int main(int argc, char *argv[])
{
 
  pid_t pid; 
  pid = fork();

	
	if(pid > 0)//父进程
	{
		for(int i(0); i<8; i++)
    {
      std::string input;
      std::cin >> input;
			pp.writeSync(input.c_str(), input.length());
		}

		int rc = ::kill(pid, SIGKILL);
		printf("kill child: %d, %d\n", pid, rc);
	}
	else if(pid == 0)//子进程
	{
		pp.readSync();
	}
	 
} 