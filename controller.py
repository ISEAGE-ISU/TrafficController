#!/usr/bin/python3
import os
import socket
import sys

sock_file = '/tmp/traffic_controller.sock'

if __name__ == '__main__':
    os.unlink(sock_file) #delete old socket file
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(sock_file)
    sock.listen(1)

    while True:
        conn, cli_addr = sock.accept()
        print("Got a connection from " + cli_addr)
        msg = conn.recv(20)
        print(msg)
