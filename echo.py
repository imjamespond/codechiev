# Echo client program
import socket
import struct 
import time
import threading

HOST = '127.0.0.1'    # The remote host
PORT = 9876              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
"""
msg1 = 'Hello, world'
header1 = struct.pack("I", len(msg1)) 
#s.send(header1)

headerBytes = bytearray(header1)
for i in range(0,4):
	print headerBytes[i]
	s.send(chr(headerBytes[i]))
	time.sleep(1) # delays
s.sendall(msg1)
"""


exitFlag = 0

class ThreadRead (threading.Thread):
    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.counter = counter
    def run(self):
        print "Starting " + self.name
        read(self.name)
        print "Exiting " + self.name

class ThreadWrite (threading.Thread):
    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.counter = counter
    def run(self):
        print "Starting " + self.name
        write(self.name)
        print "Exiting " + self.name


# Define a function for the thread
def write( threadName):
	while True:
		msg2 = raw_input(threadName+">> msg input:\n")
		header2 = struct.pack("I", len(msg2)) 
		s.sendall(header2)#blocking send
		s.sendall(msg2)
		if msg2 == "shutdown":
			break;

def read( threadName):
	while True:
		data = s.recv(4096*4096)
		print threadName+'>>Received', len(data)#repr(data)
		time.sleep(1)
		if len(data) == 0:
			break;


# Create new threads
threadRead = ThreadRead(1, "Thread-read", 1)
threadWrite = ThreadWrite(2, "Thread-write", 2)

# Start new Threads
threadRead.start()
threadWrite.start()

threadRead.join()
threadWrite.join()

s.close()

