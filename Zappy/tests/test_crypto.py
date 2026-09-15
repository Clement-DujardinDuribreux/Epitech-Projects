import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from ai.src.utils.crypto import CRYPT_PREFIX, decrypt_message, encrypt_message


class CryptoTestCase(unittest.TestCase):
    def test_encrypt_decrypt_round_trip(self) -> None:
        encrypted = encrypt_message("ZPY|team|1234|1|INV|food=9")

        self.assertTrue(encrypted.startswith(CRYPT_PREFIX))
        self.assertEqual(decrypt_message(encrypted), "ZPY|team|1234|1|INV|food=9")

    def test_decrypt_rejects_plain_and_malformed_messages(self) -> None:
        self.assertIsNone(decrypt_message("hello"))
        self.assertIsNone(decrypt_message(CRYPT_PREFIX + "!!!!"))

    def test_empty_key_keeps_payload_readable(self) -> None:
        encrypted = encrypt_message("payload", key="")

        self.assertEqual(encrypted, "payload")
        self.assertEqual(decrypt_message(CRYPT_PREFIX + "payload", key=""), "payload")


if __name__ == "__main__":
    unittest.main()
