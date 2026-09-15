"""@file crypto.py
@brief Small symmetric obfuscation helper for team broadcasts.
"""

import base64
import binascii

from ai.src.config.constants import BROADCAST_KEY


CRYPT_PREFIX = "FCW:"
_MASK = 0xFF

def encrypt_message(message: str, key: str = BROADCAST_KEY) -> str:
    """@brief Encrypt a broadcast payload with the shared team key.

    @param message Plain UTF-8 message.
    @param key Shared secret used to build the byte stream.
    @return Encoded token prefixed with `FCW:`.
    """
    if not key:
        return message
    data : bytes = message.encode("utf-8")
    key_bytes : bytes = key.encode("utf-8")
    encrypted : bytearray = bytearray()
    previous : int = seed_from_key(key_bytes)
    for index, value in enumerate(data):
        key_value : int = key_bytes[index % len(key_bytes)]
        stream : int = stream_byte(key_value, previous, index)
        encrypted_value : int = value ^ stream
        encrypted.append(encrypted_value)
        previous : int = encrypted_value
    token : str = base64.urlsafe_b64encode(bytes(encrypted)).decode("ascii").rstrip("=")
    return CRYPT_PREFIX + token


def decrypt_message(message: str, key: str = BROADCAST_KEY) -> str | None:
    """@brief Decrypt a broadcast token produced by `encrypt_message`.

    @param message Prefixed encrypted payload.
    @param key Shared secret used during encryption.
    @return Plain text message, or None when the payload cannot be decoded.
    """
    if not message.startswith(CRYPT_PREFIX):
        return None
    if not key:
        return message.removeprefix(CRYPT_PREFIX)
    token : str = message[len(CRYPT_PREFIX):]
    padding : str = "=" * (-len(token) % 4)
    try:
        data : bytes = base64.b64decode((token + padding).encode("ascii"), altchars=b"-_", validate=True)
    except (ValueError, binascii.Error):
        return None
    key_bytes : bytes = key.encode("utf-8")
    decrypted : bytearray = bytearray()
    previous : int = seed_from_key(key_bytes)
    for index, value in enumerate(data):
        key_value : int = key_bytes[index % len(key_bytes)]
        stream : int = stream_byte(key_value, previous, index)
        decrypted_value : int = value ^ stream
        decrypted.append(decrypted_value)
        previous : int = value
    try:
        return decrypted.decode("utf-8")
    except UnicodeDecodeError:
        return None


def seed_from_key(key: bytes) -> int:
    """@brief Derive the first stream state byte from a key.

    @param key Key bytes.
    @return One-byte seed value.
    """
    seed : int = 0xA7
    for index, value in enumerate(key):
        seed : int = (seed + ((value ^ (index * 31)) & _MASK)) & _MASK
        seed : int = ((seed << 3) | (seed >> 5)) & _MASK
    return seed


def stream_byte(key_value : int, previous : int, index : int) -> int:
    """@brief Generate one byte of the XOR stream.

    @param key_value Current key byte.
    @param previous Previous encrypted byte or initial seed.
    @param index Position in the message.
    @return One-byte stream value.
    """
    mixed : int = (key_value + previous + index * 17) & _MASK
    return ((mixed << 5) | (mixed >> 3)) & _MASK
