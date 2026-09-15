import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "ai" / "src"
sys.path.insert(0, str(SRC))

from elevation import Elevation, STONE_NAMES


class ElevationTestCase(unittest.TestCase):
    def test_missing_stones_only_reports_shortages(self) -> None:
        elevation = Elevation(2)
        inventory = {"linemate": 1, "deraumere": 0, "sibur": 4}

        self.assertEqual(elevation.missing_stones(inventory), {"deraumere": 1})

    def test_enough_and_any_required_stone(self) -> None:
        elevation = Elevation(1)

        self.assertFalse(elevation.has_enough_stones({"linemate": 0}))
        self.assertTrue(elevation.has_any_required_stone({"linemate": 1}))
        self.assertTrue(elevation.has_enough_stones({"linemate": 1}))

    def test_unknown_level_is_empty(self) -> None:
        elevation = Elevation(42)

        self.assertEqual(elevation.players_needed(42), 0)
        self.assertEqual(elevation.stones_to_set(42), [])
        self.assertEqual(elevation.stone_requirements(), {stone: 0 for stone in STONE_NAMES})

    def test_stones_to_set_expands_requirements(self) -> None:
        elevation = Elevation(3)

        self.assertCountEqual(
            elevation.stones_to_set(3),
            ["linemate", "linemate", "sibur", "phiras", "phiras"],
        )


if __name__ == "__main__":
    unittest.main()
