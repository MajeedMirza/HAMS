from socketIO.socketIO_client import SocketIO, LoggingNamespace

def garage(*args):
    print('garage', args[0]["open"])

socketIO = SocketIO('172.17.79.27', 3001, LoggingNamespace)
socketIO.on('garage', garage)
socketIO.wait()