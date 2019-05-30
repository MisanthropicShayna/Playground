import hashlib, binascii, os, lzma, zlib, random, json, Crypto, ast
from Crypto.PublicKey import RSA
from Crypto import Random

import base64 as ascii85


# HSE Modes
class MODE_BSE:
    ''' Uses the BSE (Byte Stream Encryption) technique to handle HSE encryption.
        Bytes to be encrypted get converted into their decimal representation.
        Example: [118,73,134,31,77,189]
    '''
    pass
class MODE_B64:
    ''' Uses the B64 (base64) technique to handle HSE encryption.
        Bytes get converted into a base64 string.
    '''
    pass
class MODE_B85:
    ''' Uses the B85 (base85) technique to handle HSE encryption.
        Bytes get converted into a base85 string.
    '''
    pass
class MODE_A85:
    ''' Uses the A85 (ascii85) technique to handle HSE encryption.
        Bytes get converted into a ascii85 string.
    '''
    pass
class MODE_B32:
    ''' Uses the B32 (base32) technique to handle HSE encryption.
        Bytes get converted into a bas32 string.
    '''
class MODE_HEX:
    ''' Uses the Hex (aka base16) technique to handle HSE encryption.
        Bytes get converted to a hexadecimal string.
    '''

def resolveMode(modeID):
    ''' Returns a mode class based on a numerical ID
        1 - BSE
        2 - HEX
        3 - B32
        4 - B64 
        5 - B85
        6 - A85
    '''

    if type(modeID) == int:
        resolver = {
            1:MODE_BSE,
            2:MODE_HEX,
            3:MODE_B32,
            4:MODE_B64,
            5:MODE_B85,
            6:MODE_A85
        }

        if modeID in resolver:
            return resolver[modeID]
        else:
            return False
    else:
        resolver = {
            MODE_BSE:1,
            MODE_HEX:2,
            MODE_B32:3,
            MODE_B64:4,
            MODE_B85:5,
            MODE_A85:6
        }

        if modeID in resolver:
            return resolver[modeID]
        else:
            return False

# HSE Encryption class
class HSE:
    ''' Provides HSE encryption with various data handling modes. '''
    encoder = None
    decoder = None

    active_mode = None

    valid_modes = [
        MODE_BSE, 
        MODE_A85, 
        MODE_B64, 
        MODE_B85,
        MODE_B32,
        MODE_HEX 
    ]

    def bytesToDecimal(self, data):
        decimal_array = []
        for byte in bytearray(data):
            decimal_array.append(byte)
        return decimal_array

    def hmacHashing(self, data, salt, iterations=10000, length=16, algorithm="sha512"):
        return binascii.hexlify(hashlib.pbkdf2_hmac(algorithm, data, salt, iterations, dklen=length)).decode()

    def initialize(self, key, mode):
        ''' Initializes the encoder for use with encryption 
            
            Param: mode
            -- The data handling mode to configure the encoder with.
            -- Valid: MODE_BSE, MODE_B64, MODE_B85, MODE_A85, MODE_B32, MODE_HEX
        '''



        if not type(key) == bytes:
            raise Exception("The specified key isn't of the bytes type.")

        if not mode in self.valid_modes:
            raise Exception("Invalid mode. Supported modes: MODE_BSE, MODE_A85, MODE_B64, MODE_B85")

        if mode == MODE_BSE:
            self.active_mode = MODE_BSE
            encoder_buffer = {}
            decoder_buffer = {}

            for byte in range(256):
                byte_str = str(byte)
                byte_hsh = self.hmacHashing(byte_str.encode(), key)

                encoder_buffer[byte] = byte_hsh
                decoder_buffer[byte_hsh] = byte

            self.encoder = encoder_buffer
            self.decoder = decoder_buffer
        if mode == MODE_B64:
            self.active_mode = MODE_B64
            encoder_buffer = {}
            decoder_buffer = {}

            character_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx yz0123456789+/="

            for char in character_set:
                char_hash = self.hmacHashing(char.encode(), key)
                encoder_buffer[char] = char_hash
                decoder_buffer[char_hash] = char

            self.encoder = encoder_buffer
            self.decoder = decoder_buffer
        if mode == MODE_B85:
            self.active_mode = MODE_B85
            encoder_buffer = {}
            decoder_buffer = {}

            character_set = "0Rs{1Op^2Lm>3Ij;4Fg*5Cd&69a#7Xy8Uv~ABDEGHJKMNPQSTVWYZb[]ce!hir-`fox)@ku%,=}qz+_lnw(?t$<|"

            for char in character_set:
                char_hash = self.hmacHashing(char.encode(), key)
                encoder_buffer[char] = char_hash
                decoder_buffer[char_hash] = char

            self.encoder = encoder_buffer
            self.decoder = decoder_buffer
        if mode == MODE_A85:
            self.active_mode = MODE_A85
            encoder_buffer = {}
            decoder_buffer = {}
            character_set = "0Rs{1Op^2Lm>3Ij;4Fg*5Cd&69a#7Xy8Uv~ABDEGHJKMNPQSTVWYZb[]ce!hir-`fox:)@.ku%,=/\}q'z+_lnw(?t$<|  abcdefghijklmnopqrstuvwx" + '"'

            for char in character_set:
                char_hash = self.hmacHashing(char.encode(), key)
                encoder_buffer[char] = char_hash
                decoder_buffer[char_hash] = char
            
            self.encoder = encoder_buffer
            self.decoder = decoder_buffer
        if mode == MODE_B32:
            self.active_mode = MODE_B32
            encoder_buffer = {}
            decoder_buffer = {}

            character_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789="

            for char in character_set:
                char_hash = self.hmacHashing(char.encode(), key)
                encoder_buffer[char] = char_hash
                decoder_buffer[char_hash] = char

            self.encoder = encoder_buffer
            self.decoder = decoder_buffer
        if mode == MODE_HEX:
            self.active_mode = MODE_HEX
            encoder_buffer = {}
            decoder_buffer = {}

            character_set = "ABCDEF1234567890="

            for char in character_set:
                char_hash = self.hmacHashing(char.encode(), key)
                encoder_buffer[char] = char_hash
                decoder_buffer[char_hash] = char

            self.encoder = encoder_buffer
            self.decoder = decoder_buffer

    def re_initialize(self, mode, encoder=False, decoder=False):
        ''' Initializes the encoder with an already generated encoder/decoder. '''
        
        if not encoder and not decoder:
            raise Exception("Either the encoder or decoder must be specified for initialiation.")
        
        if encoder and validJson(json.dumps(encoder)):
            self.encoder = encoder
        elif not validJson(json.dumps(encoder)):
            print(validJson(json.dumps(encoder)))
            print(encoder)
            raise Exception("Invalid encoder.")


        if decoder and validJson(json.dumps(decoder)):
            self.decoder = decoder
        elif not validJson(json.dumps(decoder)):
            raise Exception("Invalid decoder.")


    def encrypt(self, data, compression="zlib"):
        ''' Encrypts bytes and returns the encrypted bytes.

            Param: data
            -- Bytes of the data to be encrypted.
            -- Utilizes the mode specified during initialize to handle the data.
            -- The initialize() method must be called at least once prior to encryption.

            Param: compression (default: lzma)
            -- Compresses the encrypted data using the specified algorithm.
            -- Valid algorithms: "lzma, zlib, none"
        '''

        # Error handling
        if not self.active_mode:
            raise Exception("The encoder wasn't initialized.") 
        if not compression in ["lzma", "zlib", "none"]:
            raise Exception("Incorrect compression. Valid compression: lzma, zlib, none")


        # Encryption
        encrypted_string = ""
        if self.active_mode == MODE_BSE:
            data_decimal = self.bytesToDecimal(data)
            for byte in data_decimal:
                encrypted_string += self.encoder[byte] + " "   
        if self.active_mode == MODE_B64:
            data_b64 = ascii85.b64encode(data).decode()
            for char in data_b64:
                encrypted_string += self.encoder[char] + " "   
        if self.active_mode == MODE_B85:
            data_b85 = ascii85.b85encode(data).decode()
            for char in data_b85:
                encrypted_string += self.encoder[char] + " "      
        if self.active_mode == MODE_A85:
            data_a85 = ascii85.a85encode(data).decode()
            for char in data_a85:
                encrypted_string += self.encoder[char] + " "
        if self.active_mode == MODE_B32:
            data_b32 = ascii85.b32encode(data).decode()
            for char in data_b32:
                encrypted_string += self.encoder[char] + " "
        if self.active_mode == MODE_HEX:
            data_hex = ascii85.b16encode(data).decode()
            for char in data_hex:
                encrypted_string += self.encoder[char] + " "

        encrypted_string = encrypted_string.encode()

        # Compression
        if compression.lower() == "lzma":
            encrypted_string = lzma.compress(encrypted_string)
        if compression.lower() == "zlib":
            encrypted_string = zlib.compress(encrypted_string)
        
        return encrypted_string

    def decrypt(self, data, compression="zlib", supress_errors=True):
        '''Decrypts the specified data and returns the decrypted bytes. 

            Param: data 
            -- The data to be decrypted.

            Param: compression (default: zlib)
            -- The compression initially used when encrypting the data
            -- Valid: lzma, zlib, none

            Param: supress_errors (default: True)
            -- Choose whether or not to supress errors.
            -- When not suppressed, errors will be printed.
        '''

        # Error handling
        if not self.active_mode:
            raise Exception("The encoder wasn't initialized.")
        if not compression in ["lzma", "zlib", "none"]: 
            raise Exception("Invalid compression. Valid compression: zlib, lzma, none")

        # Decompression
        if compression == "zlib":
            try: data = zlib.decompress(data).decode()
            except: raise Exception("Failed to decompress using " + compression + ". Incorrect type?")
        elif compression == "lzma":
            try: data = lzma.decompress(data).decode()
            except: raise Exception("Failed to decompress using " + compression + ". Incorrect type?")
        else:
            data = data.decode()
            if len(data.split(" ")) in [0,1]:
                raise Exception("Data is compressed, please specify compression type.")

        # Decryption
        split_data = data.split(" ")        
        if self.active_mode == MODE_BSE:
            byte_decimal = []
            failed_hashes = []

            for Hash in split_data:
                try:
                    byte = self.decoder[Hash]
                    byte_decimal.append(byte)
                except:
                    failed_hashes.append(Hash)
            if not supress_errors and len(failed_hashes) > 0:
                print("{0} Hashes failed to be decoded. List: {1}".format(str(len(failed_hashes)), str(failed_hashes)))

            return bytes(byte_decimal)
        if self.active_mode == MODE_B64:
            b64_string = ""
            failed_hashes = []

            for Hash in split_data:
                try:
                    b64_char = self.decoder[Hash]
                    b64_string += b64_char
                except:
                    failed_hashes.append(Hash)

            if not supress_errors and len(failed_hashes) > 0:
                print("{0} Hashes failed to be decoded. List: {1}".format(str(len(failed_hashes)), str(failed_hashes)))

            return ascii85.b64decode(b64_string)
        if self.active_mode == MODE_B85:
            b85_string = ""
            failed_hashes = []

            for Hash in split_data:
                try:
                    b85_char = self.decoder[Hash]
                    b85_string += b85_char
                except:
                    failed_hashes.append(Hash)

            if not supress_errors and len(failed_hashes) > 0:
                print("{0} Hashes failed to be decoded. List: {1}".format(str(len(failed_hashes)), str(failed_hashes)))

            return ascii85.b85decode(b85_string)
        if self.active_mode == MODE_A85:
            a85_string = ""
            failed_hashes = []

            for Hash in split_data:
                try:
                    a85_char = self.decoder[Hash]
                    a85_string += a85_char
                except:
                    failed_hashes.append(Hash)

            if not supress_errors and len(failed_hashes) > 0:
                print("{0} Hashes failed to be decoded. List: {1}".format(str(len(failed_hashes)), str(failed_hashes)))

            return ascii85.a85decode(a85_string)
        if self.active_mode == MODE_B32:
            b32_string = ""
            failed_hashes = []

            for Hash in split_data:
                try:
                    b32_char = self.decoder[Hash]
                    b32_string += b32_char
                except:
                    failed_hashes.append(Hash)

            if not supress_errors and len(failed_hashes) > 0:
                print("{0} Hashes failed to be decoded. List: {1}".format(str(len(failed_hashes)), str(failed_hashes)))

            return ascii85.b32decode(b32_string)
        if self.active_mode == MODE_HEX:
            b16_string = ""
            failed_hashes = []

            for Hash in split_data:
                try:
                    b16_char = self.decoder[Hash]
                    b16_string += b16_char
                except:
                    failed_hashes.append(Hash)

            if not supress_errors and len(failed_hashes) > 0:
                print("{0} Hashes failed to be decoded. List: {1}".format(str(len(failed_hashes)), str(failed_hashes)))

            return ascii85.b16decode(b16_string)

    def __init__(self):
        pass
        
# Key generator
def generateKey(length, chars_az=True, chars_AZ=True, chars_09=True, chars_special=False):
    ''' Returns a randomly generated key based on the specified parameters.

        Param: length
        -- The length of the key

        Params: chars_az, chars_AZ, chars_09, chars_special
        -- Specify characters to use within the key     
    '''
    character_set = ""
    if chars_az: character_set += "abcdefghijlmnopqrstuvwxyz"
    if chars_AZ: character_set += "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    if chars_09: character_set += "1234567890"
    if chars_special: character_set += "!#$%&()*+-;<=>?@^_`{|}~"

    generated_key = ""
    for i in range(length):
        index = random.randint(0, len(character_set)-1)
        generated_key += character_set[index]
    return generated_key

# Bytes to Decimal/Hexadecimal array conversion
def bytesToDmal(data): 
    decimal_array = []
    for byte in bytearray(data):
        decimal_array.append(byte)
    return decimal_array   
def bytesToHexa(data):
    data_hex = ascii85.b16encode(data).decode()
    hex_array = []

    data_buffer = ""
    for b in data_hex:
        if len(data_buffer) == 2:
            hex_array.append(data_buffer)
            data_buffer = ""
        data_buffer += b

    if len(data_buffer) > 0:
        hex_array.append(data_buffer)

    return hex_array

# Byte chunk functions
def DmalChunks(data, chunksize):
    data_dmal = bytesToDmal(data)
    chunks = []
    chunk_buffer = []
    for byte in data_dmal:
        if len(chunk_buffer) >= chunksize:
            chunks.append(chunk_buffer)
            chunk_buffer = []
        chunk_buffer.append(byte)
    if len(chunk_buffer) > 0:
        chunks.append(chunk_buffer)
    return chunks
def HexaChunks(data, chunksize):
    data_hexa = bytesToHexa(data)
    chunks = []
    chunk_buffer = []
    for byte in data_hexa:
        if len(chunk_buffer) >= chunksize:
            chunks.append(chunk_buffer)
            chunk_buffer = []
        chunk_buffer.append(byte)
    if len(chunk_buffer) > 0:
        chunks.append(chunk_buffer)
    return chunks
def ByteChunks(data, chunksize):
    decimal = DmalChunks(data, chunksize)
    byte_array = []

    for chunk in decimal:
        byte_array.append(bytes(chunk))

    return byte_array
def StringChunks(data, chunksize):
    chunks = []
    chunk_buffer = []
    for char in data:
        if len(chunk_buffer) >= chunksize:
            chunks.append(chunk_buffer)
            chunk_buffer = []
        chunk_buffer.append(char)
    if len(chunk_buffer) > 0:
        chunks.append(chunk_buffer)
    return chunks

def validJson(jdata):
    if type(jdata) == bytes:
        try: jdata = jdata.decode()
        except: return False

    try: json.loads(jdata)
    except: return False
    return True

def HMAC(data, salt, iterations=100000, length=64, algorithm="sha512"):
    return binascii.hexlify(hashlib.pbkdf2_hmac(algorithm, data, salt, iterations, dklen=length)).decode()

def SHA1(data):
    return hashlib.sha1(data).hexdigest()
def SHA224(data):
    return hashlib.sha224(data).hexdigest()
def SHA256(data):
    return hashlib.sha256(data).hexdigest()
def SHA384(data):
    return hashlib.sha384(data).hexdigest()
def SHA512(data):
    return hashlib.sha512(data).hexdigest()



# Stack Cipher class
class StackCipher:
    alpha = "abcdefghijklmnopqrstuvwxyz!?@#$%^&*,"
    def encryptPhrase(self, phrase, key, scrambler):
        base = self.baseStack()
        
        stackA = []
        for stack in key.key:
            stackA.append(base[stack-1])

        for kset in scrambler.scrambler:
            fx = kset[0]-1
            fy = kset[1]-1
            tx = kset[2]-1
            ty = kset[3]-1

            f = stackA[fx][fy]
            t = stackA[tx][ty]

            stackA[tx][ty] = f
            stackA[fx][fy] = t

        alphaB = ""
        for stack in stackA:
            for char in stack:
                alphaB += char

        cacheA = ""
        for char in phrase:
            if char.lower() in self.alpha:
                cacheA += alphaB[self.alpha.index(char.lower())]
            else:
                cacheA += char

        return cacheA

    def decryptPhrase(self, phrase, key, scrambler):
        base = self.baseStack()
        
        stackA = []
        for stack in key.key:
            stackA.append(base[stack-1])

        for kset in scrambler.scrambler:
            fx = kset[0]-1
            fy = kset[1]-1
            tx = kset[2]-1
            ty = kset[3]-1

            f = stackA[fx][fy]
            t = stackA[tx][ty]

            stackA[tx][ty] = f
            stackA[fx][fy] = t

        alphaB = ""
        for stack in stackA:
            for char in stack:
                alphaB += char

        cacheA = ""
        for char in phrase:
            if char.lower() in self.alpha:
                cacheA += self.alpha[alphaB.index(char.lower())]
            else:
                cacheA += char

        return cacheA


    def baseStack(self):
        cacheA = []
        cacheB = []
        
        for char in self.alpha:
            cacheB.append(char)

            if len(cacheB) >= 4:
                cacheA.append(cacheB)
                cacheB = []

        return cacheA   
class StackKey:
    key = None
    def __init__(self, key=False):
        if not key:
            self.generate()

        else:
            self.key = []
            for char in key.split("-"):
                self.key.append(int(char))
    
    def generate(self):
        self.key = []
        for i in range(9):
            while True:
                newint = random.randint(1,9)
                if newint not in self.key:
                    self.key.append(newint)
                    break

    def getkey(self):
        return "".join([str(i)+"-" for i in self.key])[0:-1]
class StackScrambler:
    scrambler = None

    def __init__(self, scrambler=None, length=30):
        if not scrambler:
            self.generate(length)
        else:
            self.scrambler = []
            cacheA = []
            for char in scrambler:
                cacheA.append(int(char))
                
                if len(cacheA) >= 4:
                    self.scrambler.append(tuple(cacheA))
                    cacheA.clear()
                    
    def generate(self, length):
        self.scrambler = []
        for i in range(length):
            fx = random.randint(1,9)
            fy = random.randint(1,4)

            tx = random.randint(1,9)
            ty = random.randint(1,4)

            kset = (fx, fy, tx, ty)
            self.scrambler.append(kset)

    def getscrambler(self):
        cacheA = ""
        for kset in self.scrambler:
            for val in kset:
                cacheA += str(val)
        return cacheA

def rot13(message):
    ''' Applies ROT13 to a string. '''
    halfA = "abcdefghijklm"
    halfB = "nopqrstuvwxyz"

    cacheA = ""

    for char in message:
        if char.lower() != char:
            if char.lower() in halfA:
                cacheA += halfB[halfA.index(char.lower())].upper()
            elif char.lower() in halfB:
                cacheA += halfA[halfB.index(char.lower())].upper()
            else:
                cacheA += char
        else:
            if char in halfA:
                cacheA += halfB[halfA.index(char)]
            elif char in halfB:
                cacheA += halfA[halfB.index(char)]
            else:
                cacheA += char

    return cacheA
def caesar(message, offset, decode=False, alpha="abcdefghijklmnopqrstuvwxyz"):
    ''' Encodes a message using the caesar cipher using the specified offset. '''
    offset = 26 - offset
    #alpha = "abcdefghijklmnopqrstuvwxyz"
    alphaSub = alpha[-offset:] + alpha[:-offset]
    messageSub = ""

    if not decode: 
        for char in message:
            if char.lower() in alpha:
                if char != char.lower(): messageSub += alphaSub[alpha.index(char.lower())].upper()
                else: messageSub += alphaSub[alpha.index(char)]
            else: messageSub += char
    else:
        for char in message:
            if char.lower() in alphaSub:
                if char != char.lower(): messageSub += alpha[alphaSub.index(char.lower())].upper()
                else: messageSub += alpha[alphaSub.index(char)]
            else: messageSub += char

    return messageSub
def vigenere(message, key, alpha="abcdefghijklmnopqrstuvwxyz", decode=False):
    ''' Encodes a message using the vigenere cipher and the specified key. '''

    table = []
    for i in range(len(alpha)):
        alphaSub = alpha[i:] + alpha[:i]
        table.append(alphaSub)
    
    if decode:
        key_index = -1
        messageCache = ""
        for char in message:
            if char.lower() in alpha:
                key_index += 1
                if key_index > len(key)-1: key_index = 0
                uppercase = False
                if char != char.lower(): uppercase = True

                targetChar = alpha[table[alpha.index(key[key_index].lower())].index(char.lower())]
                if uppercase: messageCache += targetChar.upper()
                else: messageCache += targetChar
            else:
                messageCache += char
        return messageCache

    else:
        messageCache = ""
        key_index = -1
        for char in message:
            if char.lower() in alpha:    
                key_index += 1
                if key_index > len(key)-1:
                    key_index = 0
    
                uppercase = False
                letterMessage = char
                letterKey = key[key_index].lower()

                # Checks if the character is meant to be uppercase
                if letterMessage != letterMessage.lower():
                    uppercase = True
                    letterMessage = letterMessage.lower()


                index_letterMessage = alpha.index(letterMessage)
                index_letterKey = alpha.index(letterKey)

                letterCrossed = table[index_letterMessage][index_letterKey]

                if uppercase: 
                    letterCrossed = letterCrossed.upper()

                messageCache += letterCrossed

            else:
                messageCache += char

        return messageCache


class RSAEncryptor:
    PublicKey = None

    def __init__(self, publicKey):
        self.PublicKey = RSA.importKey(publicKey)
    
    def decrypt(self, data, randfactor=32):
        return self.PublicKey.encrypt(data, randfactor)

class RSADecryptor:
    PrivateKey = None
    
    def __init__(self, privateKey):
        self.PrivateKey = RSA.importKey(privateKey)

    def encrypt(self, data, randfactor=32):
        return self.PrivateKey.encrypt(data, randfactor)

def GenRSAKeySet(keylength=1024):
    RandomFunction = Random.new().read
    PPKeySet = RSA.generate(keylength, RandomFunction)
    public = PPKeySet.publickey().exportKey()
    private = PPKeySet.exportKey()
    return public, private

class RSA_KeySet:
    public, private = None, None
    def __init__(self, public, private):
        self.public = public
        self.private = private
    
        