import socket
import time
import Paradigm_pb2

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    receivedInfo = Paradigm_pb2.Person()

    server_address = ('localhost', 10000)
    sock.bind(server_address)

    while True:
        data, sender_info = sock.recvfrom(256)
        print(data[:4])
        messageLength = int.from_bytes(data[:4], 'little')
        print('received {} bytes'.format(messageLength))
        messageData = data[4:4 + messageLength]

        if receivedInfo.ParseFromString(messageData):
            print('parsed data!!!')
            print('Name: {}'.format(receivedInfo.name))
            print('Email: {}'.format(receivedInfo.email))
            print('echoeing back')
            returnMessage = messageLength.to_bytes(4, 'little') + receivedInfo.SerializeToString()
            sock.sendto(returnMessage, sender_info)
        else:
            print('invalid data!')
            break

if __name__ == '__main__':
    main()
