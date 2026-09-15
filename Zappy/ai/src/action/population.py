"""@file population.py
@brief Team population management and egg creation.
"""

from ai.src import clients

from ai.src.clients import DataType as ResponseType
from ai.src.config.constants import (
    FOOD_CRITICAL,
    FORK_COOLDOWN_TICKS,
    POPULATION_CHECK_INTERVAL,
    TEAM_TARGET_SIZE,
)

class Population:
    """@brief Mixin handling captain election and team growth."""

    def captain_id(self) -> int:
        """@brief Return the smallest known agent id."""
        return min([self.agent_id, *self.new_mates().keys()])

    def is_captain(self) -> bool:
        """@brief Check whether this agent should act as captain."""
        if self.tick < POPULATION_CHECK_INTERVAL:
            return False
        return self.agent_id == self.captain_id()

    def population_step(self) -> bool:
        """@brief Run one population-management decision.

        @return True when the step consumed the current tick's action.
        """
        if not self.population_enabled or TEAM_TARGET_SIZE <= 1:
            return False
        if not self.is_captain() or self.mode != "free" or self.incanting:
            return False
        missing : int = TEAM_TARGET_SIZE - self.projected_team_size()
        if missing <= 0:
            return False
        if self.tick - self.last_population_check < POPULATION_CHECK_INTERVAL:
            return False
        self.last_population_check = self.tick
        slots : int | None = self.free_slot_count()
        if slots is not None:
            self.free_slots = max(0, slots)
        missing : int = TEAM_TARGET_SIZE - self.projected_team_size()
        if missing <= 0:
            return False
        if self.free_slots > 0:
            return False
        if self.tick - self.last_fork_tick < FORK_COOLDOWN_TICKS:
            return False
        if self.food() < max(18, FOOD_CRITICAL + 6):
            return False
        self.last_fork_tick : int = self.tick
        response : clients.Response | None = self.fork()
        if response and response.data_type == ResponseType.OK:
            self.free_slots += 1
            return True
        if response and response.data_type == ResponseType.KO:
            self.free_slots = 0
        return True

    def free_slot_count(self) -> int | None:
        """@brief Query the server for available team slots.

        @return Slot count when the server response is valid, otherwise None.
        """
        response : clients.Response | None = self.connect_nbr()
        if response and response.data_type == ResponseType.VALUE and isinstance(response.data, int):
            return response.data
        return None
