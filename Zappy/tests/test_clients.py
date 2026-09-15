import sys
import unittest
from pathlib import Path
from unittest.mock import Mock, patch


ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "ai" / "src"
sys.path.insert(0, str(SRC))

import clients
from clients import Client, DataType


class ClientParsingTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.socket_patcher = patch.object(clients.socket, "socket")
        socket_factory = self.socket_patcher.start()
        socket_factory.return_value = Mock()
        self.client = Client("127.0.0.1", 4242)

    def tearDown(self) -> None:
        self.socket_patcher.stop()

    def test_parse_status_lines_decrement_pending(self) -> None:
        self.client.pendingCount = 2

        ok = self.client.parseLine("ok")
        ko = self.client.parseLine("ko")

        self.assertEqual(ok.data_type, DataType.OK)
        self.assertEqual(ko.data_type, DataType.KO)
        self.assertEqual(self.client.pendingCount, 0)

    def test_parse_inventory_response(self) -> None:
        self.client.pendingCount = 1

        response = self.client.parseLine("[food 9, linemate 1, thystame 0]")

        self.assertEqual(response.data_type, DataType.INVENTORY)
        self.assertEqual(response.data, {"food": 9, "linemate": 1, "thystame": 0})
        self.assertEqual(self.client.pendingCount, 0)

    def test_parse_look_response_counts_items_per_tile(self) -> None:
        self.client.pendingCount = 1

        response = self.client.parseLine("[player food, linemate linemate, ]")

        self.assertEqual(response.data_type, DataType.LOOK)
        self.assertEqual(response.data[0], {"player": 1, "food": 1})
        self.assertEqual(response.data[1], {"linemate": 2})

    def test_parse_broadcast_and_eject_events(self) -> None:
        broadcast = self.client.parseLine("message 3, hello team")
        eject = self.client.parseLine("eject: 7")

        self.assertEqual(broadcast.data_type, DataType.BROADCAST)
        self.assertEqual(broadcast.data, {"direction": 3, "text": "hello team"})
        self.assertEqual(eject.data_type, DataType.EJECT)
        self.assertEqual(eject.data, 7)


if __name__ == "__main__":
    unittest.main()
