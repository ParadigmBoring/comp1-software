import socket
import time
import Paradigm_pb2

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    receivedInfo = Paradigm_pb2.Person()

    server_address = ('127.0.0.1', 10000)
    sock.bind(server_address)

    sock.listen(1)

    while True:
        print('waiting for a connection!')
        connection, client_address = sock.accept()
        try:
            print('connection_from {}'.format(client_address))

            while True:
                data = connection.recv(4)
                messageLength = int.from_bytes(data, 'little')
                messageData = connection.recv(messageLength)
                print('received {} bytes'.format(messageLength))

                if messageData:
                    if receivedInfo.ParseFromString(messageData):
                        print('parsed data!!!')
                        print('Name: {}'.format(receivedInfo.name))
                        print('Email: {}'.format(receivedInfo.email))
                        print('echoeing back')
                        returnMessage = messageLength.to_bytes(4, 'little') + receivedInfo.SerializeToString()
                        connection.send(returnMessage)
                    else:
                        print('invalid data!')
                        break
                else:
                    print('no more data from {}'.format(client_address))
                    break
        finally:
            connection.close()

if __name__ == '__main__':
    main()
