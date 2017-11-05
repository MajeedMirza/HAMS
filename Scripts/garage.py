from threading import Thread
from socketIO.socketIO_client import SocketIO, LoggingNamespace

test = 0

def garage(*args):
    print('garage', args[0]["open"])
    print test

def _receive_events_thread():
    socketIO.wait()

socketIO = SocketIO('172.17.79.27', 3001, LoggingNamespace)
socketIO.on('garage', garage)
receive_events_thread = Thread(target=_receive_events_thread)
receive_events_thread.daemon = True
receive_events_thread.start()


while True:
    test = test + 1