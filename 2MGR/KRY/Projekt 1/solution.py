#!/usr/bin/env python3

BIS_FILE = 'in/bis.txt'
BIS_FILE_ENCRYPTED = 'in/bis.txt.enc'
GIF_ENCRYPTED = 'in/hint.gif.enc'
SCRIPT_ENCRYPTED = 'in/super_cipher.py.enc'


''' FOLLOWING CODE IS FROM DECRYPTED super_cipher.py.enc FILE '''
SUB = [0, 1, 1, 0, 1, 0, 1, 0]
N_B = 32
N = 8 * N_B


# Next keystream
def step(x):
    x = (x & 1) << N+1 | x << 1 | x >> N-1
    y = 0
    for i in range(N):
        y |= SUB[(x >> i) & 7] << i
    return y
'''-----------------------------------------------------------'''


# reverse step
def reverse_step(y):
    # every value can have 4 images
    images = []

    # check SUB array
    for x in range(len(SUB)):
        if SUB[x] == y & 1:  # check last bit
            images.append(x)  # append to array if last bit matches

    # loop each bit
    for x in range(N):
        tmp_images = []
        for z in images:
            for i in range(len(SUB)):
                if SUB[i] == (y >> x) & 1:  # check last bit
                    if (i & 3) == (z >> x) & 3:  # check two bits
                        z |= (i << x)
                        tmp_images.append(z)  # append into tmp images
        images = tmp_images  # set new images

    # loop images
    for x in images:
        if (x >> 256) == (x & 3):  # first two bits have to match last two bits
            return (x >> 1) & ((1 << N) - 1)  # return reversed value

    # otherwise print error and end script
    print("Could not reverse the key!")
    exit(1)


# decrypt part of super_cipher.py.enc file stored in folder 'in' (this was the first step to crack the cipher)
# after decrypting this file I was able to copy the 'step' function and constants
def decrypt_super_cipher_file_partial():
    bis_txt = open(BIS_FILE, "rb").read()
    bis_enc = open(BIS_FILE_ENCRYPTED, "rb").read()
    script_enc = open(SCRIPT_ENCRYPTED, "rb").read()

    # get the key stream used to encrypt the message
    key_stream = [a ^ b for (a, b) in zip(bis_txt, bis_enc)]

    # decrypt the script
    decrypted_script = [a ^ b for (a, b) in zip(script_enc, key_stream)]

    # write into file
    output = open('super_cipher_partial.py', 'wb')
    output.write(bytes(decrypted_script))
    output.close()


# decrypt the super_cipher.py.enc file
# after getting functions from partial super_cipher.py file i was able to generate the key stream end decrypt the file
def decrypt_super_cipher_file_full():
    # read first 32 bytes from each file
    bis_txt = open(BIS_FILE, "rb").read(N_B)
    bis_enc = open(BIS_FILE_ENCRYPTED, "rb").read(N_B)

    # get the key stream
    key_stream = [a ^ b for (a, b) in zip(bis_txt, bis_enc)]

    # open encrypted script
    script_enc = open(SCRIPT_ENCRYPTED, "rb")

    # encrypt
    output = open('super_cipher.py', 'wb')
    enc_byte = script_enc.read(N_B)
    while enc_byte:
        # decrypt bytes
        decrypted_bytes = [a ^ b for (a, b) in zip(enc_byte, key_stream)]

        # write bytes into file
        output.write(bytes(decrypted_bytes))

        # get next key
        key_stream = step(int.from_bytes(key_stream, 'little')).to_bytes(N_B, 'little')

        # get next byte
        enc_byte = script_enc.read(N_B)

    # close file
    output.close()


# decrypt the hint.gif.enc file
# after getting functions from partial super_cipher.py file i am able to generate the key stream end decrypt the file
def decrypt_gif_file():
    # read first 32 bytes from each file
    bis_txt = open(BIS_FILE, "rb").read(N_B)
    bis_enc = open(BIS_FILE_ENCRYPTED, "rb").read(N_B)

    # get the key stream
    key_stream = [a ^ b for (a, b) in zip(bis_txt, bis_enc)]

    # open encrypted script
    gif_enc = open(GIF_ENCRYPTED, "rb")

    # encrypt
    output = open('hint.gif', 'wb')
    enc_byte = gif_enc.read(N_B)
    while enc_byte:
        # decrypt bytes
        decrypted_bytes = [a ^ b for (a, b) in zip(enc_byte, key_stream)]

        # write bytes into file
        output.write(bytes(decrypted_bytes))

        # get next key
        key_stream = step(int.from_bytes(key_stream, 'little')).to_bytes(N_B, 'little')

        # get next byte
        enc_byte = gif_enc.read(N_B)

    # close file
    output.close()


# get cipher secret
def get_cipher_secret():
    bis_txt = open(BIS_FILE, "rb").read(N_B)
    bis_enc = open(BIS_FILE_ENCRYPTED, "rb").read(N_B)

    # get first 32 bytes from keystream
    key = int.from_bytes(bis_txt, 'little') ^ int.from_bytes(bis_enc, 'little')

    # loop 128 times and reverse the keystream
    for i in range(0, 128):
        key = reverse_step(key)

    # print key
    print(key.to_bytes(N_B, 'little').decode(), end='')


# run script
if __name__ == '__main__':
    get_cipher_secret()
