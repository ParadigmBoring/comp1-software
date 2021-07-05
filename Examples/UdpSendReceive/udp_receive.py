""" udp_receive.py

Receive and print a message via UDP
"""

import socket

from config import *

if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET,    # Internet
                         socket.SOCK_DGRAM) # UDP
    sock.bind((UDP_IP, UDP_PORT))

    while True:
        data, addr = sock.recvfrom(1024) # Buffer size is 1024 bytes
        print(f"Received message: {data}")
