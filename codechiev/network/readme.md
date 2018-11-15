# TCP server  
channel 指针相关操作(send, shutdown) 只能在loop线程中执行才能确保指针安全,  
若要在loop外部操作channel必须将channel拷贝一份  

# 定时器  
schedule 会打断之前重复执行的task