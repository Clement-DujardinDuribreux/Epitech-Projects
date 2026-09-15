import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "ai" / "src"
sys.path.insert(0, str(SRC))

from navigation import Navigation


class NavigationTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.navigation = Navigation()

    def test_tile_index_to_coords_origin_and_rows(self) -> None:
        self.assertEqual(self.navigation.tile_index_to_coords(0), (0, 0))
        self.assertEqual(self.navigation.tile_index_to_coords(-4), (0, 0))
        self.assertEqual(self.navigation.tile_index_to_coords(1), (1, -1))
        self.assertEqual(self.navigation.tile_index_to_coords(2), (1, 0))
        self.assertEqual(self.navigation.tile_index_to_coords(3), (1, 1))
        self.assertEqual(self.navigation.tile_index_to_coords(8), (2, 2))

    def test_moves_to_tile(self) -> None:
        self.assertEqual(self.navigation.moves_to_tile(0), [])
        self.assertEqual(self.navigation.moves_to_tile(1), ["Forward", "Left", "Forward"])
        self.assertEqual(self.navigation.moves_to_tile(2), ["Forward"])
        self.assertEqual(self.navigation.moves_to_tile(3), ["Forward", "Right", "Forward"])

    def test_broadcast_direction_returns_copy(self) -> None:
        moves = self.navigation.moves_from_broadcast_direction(6)
        moves.append("Left")

        self.assertEqual(self.navigation.moves_from_broadcast_direction(6), ["Right", "Forward"])
        self.assertEqual(self.navigation.moves_from_broadcast_direction(99), [])


if __name__ == "__main__":
    unittest.main()
