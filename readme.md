## vscode远程开发  
mac 上安装 osxfuse / sshfs 将远程workspace的路径映射至本地
https://github.com/osxfuse/sshfs  
`mkdir ???-include` 建立远程头文件路径
`mount --bind /usr/local/include local-include` 将远程系统的头文件映射至workspace  
将 `local-include` 添加至vscode的setting中

## 关于pthread
``./configure ``  
...  
checking whether pthreads work with -pthread... yes  
``#include <event2/thread.h>``  
``link with  libevent_pthreads``  
evthread_use_pthreads()  

## make shell  
通过设置env来调节日志级别  
loglevel=Info ./make.sh 或 export loglevel=Info && ./make.sh

## $'\r': command not found  
Notepad++. From the "Edit" menu, select "EOL Conversion" -> "UNIX/OSX Format".   
## git old mode new mode  
git config core.filemode false  
## git different new line issue  
.gitattributes  
*        -crlf