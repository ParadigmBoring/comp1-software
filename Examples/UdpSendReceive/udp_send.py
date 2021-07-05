""" udp_send.py

Send a simple message over UDP.
"""

import socket
import time

from config import *

if __name__ == "__main__":
    MESSAGE = b"Hello, World!"

    print(f"UDP target IP: {UDP_IP}")
    print(f"UDP target port: {UDP_PORT}")
    print(f"Message: {MESSAGE}\n")

    sock = socket.socket(socket.AF_INET,    # Internet
                         socket.SOCK_DGRAM) # UDP
    
    while True:
        sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
        print("Message sent...\n")
        time.sleep(1/SEND_FREQUENCY)
    