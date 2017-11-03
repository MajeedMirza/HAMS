from socket import *


socket = socket(AF_INET, SOCK_DGRAM)
socket.settimeout(1)
addr = ("127.0.0.1", 2000)
socket.sendto("{alarm: string}", addr)
## WAIT FOR SEVER ACK HERE if not try again
message, address = socket.recvfrom(1024)
print(message)

## send alarm to other nodes
## if alarm is reseted send a reset

