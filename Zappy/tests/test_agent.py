import sys
import unittest
from pathlib import Path
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from ai.src.agent import Agent
from ai.src.clients import DataType, ServerResponse
from ai.src.models.session import Mate


class DummyClient:
    def __init__(self) -> None:
        self.commands: list[str] = []
        self.events: list[ServerResponse] = []
        self.maxPending = 10
        self.ip = "127.0.0.1"
        self.port = 4242

    def command(self, command: str) -> ServerResponse:
        self.commands.append(command)
        return ServerResponse(DataType.OK, None)

    def popEvents(self) -> list[ServerResponse]:
        events = self.events
        self.events = []
        return events

    def poll(self) -> list[ServerResponse]:
        return []


class AgentStateTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.client_patcher = patch("ai.src.agent.ZappyClient", lambda ip, port: DummyClient())
        self.client_patcher.start()
        self.agent = Agent("127.0.0.1", 4242, "team")

    def tearDown(self) -> None:
        self.client_patcher.stop()

    def test_spend_for_command_updates_food_estimate(self) -> None:
        self.agent.food_estimate = 10.0
        self.agent.inventory_data["food"] = 10

        self.agent.spend_for_command("Forward")

        self.assertLess(self.agent.food_estimate, 10.0)
        self.assertEqual(self.agent.inventory_data["food"], int(self.agent.food_estimate))

    def test_take_and_set_item_update_local_inventory(self) -> None:
        self.assertTrue(self.agent.take_item("linemate"))
        self.assertEqual(self.agent.inventory_data["linemate"], 1)

        self.assertTrue(self.agent.set_item("linemate"))
        self.assertEqual(self.agent.inventory_data["linemate"], 0)

    def test_new_mates_removes_timed_out_entries(self) -> None:
        self.agent.tick = 1000
        self.agent.mates = {
            12: Mate(1, {"food": 5}, 100),
            34: Mate(1, {"food": 5}, 990),
        }

        mates = self.agent.new_mates()

        self.assertNotIn(12, mates)
        self.assertIn(34, mates)

    def test_population_fork_does_not_spawn_child_process(self) -> None:
        self.agent.tick = 1000
        self.agent.agent_id = 1
        self.agent.last_population_check = -999
        self.agent.last_fork_tick = -999
        self.agent.food_estimate = 80.0
        self.agent.inventory_data["food"] = 80
        self.agent.free_slots = 0
        self.agent.mates = {}

        self.assertTrue(self.agent.population_step())

        self.assertIn("Fork", self.agent.client.commands)
        self.assertEqual(self.agent.free_slots, 1)


if __name__ == "__main__":
    unittest.main()
