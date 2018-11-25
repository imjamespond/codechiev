# Tcp 
当shutdown WR后, socket将发送fin给对方并进入FIN-WAIT-2模式, 等待关闭; 另一方则进入CLOSE-WAIT模式;
如果用nc AKA netcat 测试, 其可能不会发送fin值, 因此必须手动ctl+d关闭stdin

# TCP server  
channel 指针相关操作(send, shutdown) 只能在loop线程中执行才能确保指针安全,  
若要在loop外部操作channel必须将channel拷贝一份  

# 定时器  
schedule 会打断之前重复执行的task