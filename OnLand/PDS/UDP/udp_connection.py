""" udp_connection.py

Classes:
    UDPListener - Wrapper around a uni-directional UDP connection
"""
import logging
import socket
import select
from datetime import timedelta


class UDPListener:
    """ UDP Listener  

    Uni-directional UDP listening on specified ip and port

    Fields:
        ip: string - connection address
        port: int - port to listen on
        max_buffer: int - max byte size of the UDP receive/send buffers 
        timeout: timedelta - how long to poll for data on the socket before timing out
    """

    def __init__(self, ip : str, port : int, max_buffer : int, timeout : timedelta):
        self.sock = None
        self.ip = ip
        self.port = port
        self.max_buffer_size = max_buffer
        self.timeout = timeout

    @classmethod
    def transcribe(self, data : bytes):
        """ Log the received byte data """
        logging.info(f"[DATA] {data}")
    
    def connect(self):
        """ Connect to socket """
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, self.max_buffer_size)
            self.sock.bind((self.ip, self.port))
        except Exception as e:
            self.disconnect()
            raise e

    def is_connected(self):
        """ Returns true if connected """
        connected = self.sock is not None and self.sock.fileno() >= 0
        return connected

    def disconnect(self):
        """ Disconnect from socket """
        if self.sock is not None:
            self.sock.close()
            self.sock = None

    def recv(self):
        """ Receive UDP packet """
        if not self.is_connected():
            return None
        try:
            ready = select.select([self.sock], [], [], self.timeout.total_seconds())
            if ready[0]:
                data = self.sock.recvfrom(self.max_buffer_size)
                if data is not None:
                    return data[0]
            else:
                return None

        except Exception as e:
            self.disconnect()
            raise e

    def __str__(self):
        ret_str = f"""UDP Connection:
        \nIp: {self.ip}
        \nPort: {self.port}
        \nBuffer Size: {self.max_buffer_size}
        """
        return ret_str

if __name__ == "__main__":
    # Vehicle ECU
    VEHICLE_IP = "127.0.0.1"
    VEHICLE_UDP_PORT = 5005

    con = UDPListener(VEHICLE_IP, VEHICLE_UDP_PORT, 1024, timedelta(seconds=2))
    con.connect()

    counter = 5
    while True:
        data = con.recv()
        if data is not None:
            print(f"Data:{data.decode('utf-8')}")
        else:
            print("No data...")
    
