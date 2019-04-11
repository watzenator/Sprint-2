import socket

s = socket.socket()
s.connect(('127.0.0.1',12348))
while True:
    str = raw_input("S: ")
    s.send(str.encode());
    if(str == "!"):
        break
s.close()
