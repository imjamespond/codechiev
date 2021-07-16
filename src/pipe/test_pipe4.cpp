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
	std::stringstream ss;
	ss << "before fork" << std::endl;
	::write(STDOUT_FILENO, ss.str().c_str(), ss.str().length());
  
	// https://stackoverflow.com/questions/7253659/why-the-address-of-variable-of-child-process-and-parent-process-is-same
  pid_t pid; // virtual address is the same in child process, but value is not!
  pid = fork();

	ss.str("");
	ss << "fork pid: " << pid << ", addr: " << &pid << std::endl;
	::write(STDOUT_FILENO, ss.str().c_str(), ss.str().length());
	
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