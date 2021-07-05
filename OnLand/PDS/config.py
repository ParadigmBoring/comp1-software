""" config.py

Configuration values for:
    - InfluxDB
    - Vehicle ECU TCP/UDP Connections
    - Surface ECU TCP/UDP Connections
"""

# Influx
INFLUX_HOST = "localhost"
INFLUX_PORT = 8086
INFLUX_NAME = "paradigm_boring"
INFLUX_USER = "paradigm"
INFLUX_PW = "boring"

# TBC Packet
TEAM_NAME = "Paradigm Boring"

# SocketIO 
SOCKET_SERVER = "http://localhost:5000"

# Vehicle ECU
VEHICLE_IP = "127.0.0.1"
VEHICLE_UDP_PORT = 5005

# Surface ECU
SURFACE_IP = "127.0.0.1"
SURFACE_UDP_PORT = 5006
