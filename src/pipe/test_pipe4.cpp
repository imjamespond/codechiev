#include <stdio.h>  // perror
#include <stdlib.h> // exit 
#include <signal.h> // kill
#include <sys/types.h> // SIGTERAM
#include <string>
#include <iostream>
#include <string.h> // memset,
#include <sys/wait.h>  

#include <epoll/pipe.hpp> 
#include <utils/log.hpp> 

learn_cpp::Pipe pp;

pid_t p1();
pid_t p2(pid_t);

int main(int argc, char *argv[])
{ 
	pid_t pid1, pid2;
	if ((pid1 = p1()) != 0) {
		if ((pid2 = p2(pid1)) != 0) {
			Log() << "waitpid: " << pid1 << "," << pid2; 
			int wstatus(0);
			::waitpid(pid1, &wstatus, 0); 
			Log() << "pid1 exit";
			::waitpid(pid2, &wstatus, 0); 
			Log() << "pid2 exit";
		}
	} 
}

pid_t p1()
{ 
	Log() << "before fork";
  
	// https://stackoverflow.com/questions/7253659/why-the-address-of-variable-of-child-process-and-parent-process-is-same
  pid_t pid; // virtual address is the same in child process, but value is not!
  pid = fork();
 
	Log() << "fork pid: " << pid << ", addr: " << &pid; 
	
	if(pid == 0)//子进程
	{
		int len(0), total(0); 
		char buf[2];
		const int bufLen = sizeof(buf);
		
		while (total < 10)
		{
			::memset(buf, 0, bufLen);
			len = pp.readSync(buf, bufLen);
			if (len > 0)
			{
				total += len;
				std::string str(buf, buf + len); 
				Log() << "buf: " << str.c_str() << ", len: " << len ; 
			} 
		}
	}
	return pid;
} 


pid_t p2(pid_t pid1)
{ 
	Log() << "before fork" ; 
  
	// https://stackoverflow.com/questions/7253659/why-the-address-of-variable-of-child-process-and-parent-process-is-same
  pid_t pid; // virtual address is the same in child process, but value is not!
  pid = fork(); 
 
	Log() << "fork pid: " << pid << ", addr: " << &pid ; 
	
	
	if(pid == 0)//子进程
	{
		while(1)
    {
      std::string input;
      std::cin >> input;
			if (input == "q") {
				int rc = ::kill(pid1, SIGKILL);
				Log() << "kill pid1:" << pid1 << ", " << rc;  
				break;
			} else {
				pp.writeSync(input.c_str(), input.length());
			}
		}
	}

	return pid;
} 