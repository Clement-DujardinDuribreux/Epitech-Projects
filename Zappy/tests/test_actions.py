import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from ai.src.action.foraging import Foraging
from ai.src.action.ladder import Ladder
from ai.src.elevation import STONE_NAMES
from ai.src.navigation import Navigation


class DummyAgent(Foraging, Ladder):
    def __init__(self) -> None:
        self.level = 1
        self.navigator = Navigation()
        self.inventory_data = {stone: 0 for stone in STONE_NAMES}
        self.set_calls: list[str] = []
        self.take_calls: list[str] = []

    def set_item(self, item: str) -> bool:
        self.set_calls.append(item)
        self.inventory_data[item] -= 1
        return True

    def take_item(self, item: str) -> bool:
        self.take_calls.append(item)
        self.inventory_data[item] += 1
        return True

    def food(self) -> float:
        return 50.0


class ActionMixinTestCase(unittest.TestCase):
    def test_best_visible_food_tile_uses_count_and_distance(self) -> None:
        agent = DummyAgent()
        vision = [
            {},
            {"food": 1},
            {"food": 1},
            {"food": 3},
        ]

        self.assertEqual(agent.best_visible_food_tile(vision), 3)

    def test_bank_stone_prefers_missing_rare_resource(self) -> None:
        agent = DummyAgent()
        agent.level = 7
        tile = {"linemate": 1, "thystame": 1}

        self.assertEqual(agent.best_bank_stone_on_tile(tile), "thystame")

    def test_prepare_ladder_tile_sets_missing_stones_and_removes_extra(self) -> None:
        agent = DummyAgent()
        agent.level = 1
        agent.inventory_data["linemate"] = 1
        tile = {"deraumere": 1}

        self.assertTrue(agent.prepare_ladder_tile(tile))
        self.assertEqual(agent.set_calls, ["linemate"])
        self.assertEqual(agent.take_calls, ["deraumere"])
        self.assertEqual(tile["linemate"], 1)
        self.assertEqual(tile["deraumere"], 0)


if __name__ == "__main__":
    unittest.main()
