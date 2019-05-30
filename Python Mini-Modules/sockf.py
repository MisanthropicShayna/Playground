import socket, bytef

def transmit(data, address, chunksize=1024):
    transmitSocket = socket.socket() # Creates the transmission socket.
    transmitSocket.connect(address) # Connects the socket to the destination address.

    # Splits the specified data into chunks of the specified chunksize
    dataChunks = bytef.DmalChunks(data, chunksize)

    # Transmits every chunk of data in dataChunks one by one to the destination address.
    for chunk in dataChunks:
        chunkBytes = bytes(chunk)
        transmitSocket.send(chunkBytes)

    # Attempts to close the socket after the data was transmitted.
    try: transmitSocket.close()
    except: pass


def recieveUDP(recieveSocket, blacklist=None, whitelist=None):
    recieved_bytes = []
    address = None
    while True:
        try:
            chunk, source = recieveSocket.recvfrom(1024)
            if whitelist:
                if source[0] not in whitelist:
                    break
            if blacklist:
                if source[0] in blacklist:
                    break

            if not address: address = source
            if not chunk: break
            if chunk == b'\0': break
            for byte in bytef.bytesToDmal(chunk):
                recieved_bytes.append(byte)
        except: break

    if not address:
        return -1
    return bytes(recieved_bytes), address
def transmitUDP(data, address, chunksize=1024, timeout=1.0):
    transmitSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Creates the transmission socket
    transmitSocket.settimeout(timeout) # Sets the timeout period if the data cannot be transmitted

    # Splits the specified data into decimal byte chunks of the specified chunksize
    dataChunks = bytef.DmalChunks(data, chunksize)
    
    # Transmits every chunk of data in dataChunks as bytes one by one to the destination address.
    for chunk in dataChunks:
        chunkBytes = bytes(chunk)
        transmitSocket.sendto(chunkBytes, address)
    transmitSocket.sendto(b'\0', address)

    # Attempts to close the socket after the data was transmitted. 
    try: transmitSocket.close()
    except: pass

def recieve(recieveSocket, blacklist=None, whitelist=None):
    recieveSocket.listen(1) # Sets the socket up to listen for incomming connections.
    connection, source = recieveSocket.accept() # Accepts an incomming connection.
    recievedBytes = [] # Creates an array soon to be filled with the recieved bytes.


    # Closes the connection if the source is part of a blacklist, if one is specified.
    if blacklist:
        if source[0] in blacklist:
            try: connection.close()
            except: pass
            return -1, source
            
    if whitelist:
        if source[0] not in whitelist:
            try: connection.close()
            except: pass
            return -1, source

    # Recieves incomming 1024 byte chunks and compiles them into recievedBytes.
    while True:
        try:
            chunk = connection.recv(1024)
            for byte in bytef.bytesToDmal(chunk):
                recievedBytes.append(byte)
            if not chunk: break
        except: pass

    # Closes the connection and returns the recieved bytes
    try: connection.close()
    except: pass
    return bytes(recievedBytes), source
    

# Standard TCP socket for server use
standardTCP = lambda: socket.socket(socket.AF_INET, socket.SOCK_STREAM)
standardUDP = lambda: socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def recieveon(recieveAddress, blacklist=None, whitelist=None):
    ''' Works similar to the standard recieve function, but rather than passing a TCP socket to recieve from,
    an address is passed instead, a temporary TCP socket will then automatically be created and bound to the given
    address, and then closed afterwards. This is better suited for temporary data reception.'''

    TCP_RecieveSocket = standardTCP()
    TCP_RecieveSocket.bind(recieveAddress)
    recv_data, recv_source = recieve(TCP_RecieveSocket, blacklist, whitelist)
    try: 
        TCP_RecieveSocket.close()
        TCP_RecieveSocket = None
    except: pass
    return recv_data, recv_source
