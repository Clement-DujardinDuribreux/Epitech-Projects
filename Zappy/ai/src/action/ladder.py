"""@file ladder.py
@brief Incantation coordination logic for captain and follower agents.
"""

from ai.src.clients import ServerResponse as Response, DataType as ResponseType
from ai.src.config.constants import (
    CALL_BROADCAST_INTERVAL,
    CAPTAIN_FOOD_TARGET,
    CAPTAIN_INCANTATION_ABORT,
    CAPTAIN_INCANTATION_MIN,
    FOOD_CRITICAL,
    INVENTORY_INTERVAL,
    MATE_TIMEOUT,
    SESSION_TIMEOUT,
    SUPPORT_FOOD_TARGET,
    SUPPORT_INCANTATION_MIN,
    TEAM_TARGET_SIZE,
)
from ai.src.elevation import ELEVATION_TABLE, STONE_NAMES
from ai.src.models.session import Session


class Ladder:
    """@brief Mixin that coordinates multi-player level-up sessions."""

    def team_ready_for_ladder(self) -> bool:
        """@brief Check whether enough known teammates can join an incantation.

        @return True when team size and food thresholds are satisfied.
        """
        if TEAM_TARGET_SIZE <= 1:
            return True
        if self.food() < CAPTAIN_INCANTATION_MIN:
            return False
        ready : int = 1
        for mate in self.new_mates().values():
            if mate.level != self.level:
                continue
            if int(mate.inventory.get("food", 0)) < SUPPORT_INCANTATION_MIN:
                continue
            ready += 1
        return ready >= TEAM_TARGET_SIZE

    def start_ladder_session(self) -> None:
        """@brief Switch the current agent into ladder master mode."""
        session_id : str = f"BANK-{self.level}-{self.agent_id}-{self.tick}"
        self.session : Session = Session(session_id, self.level, self.agent_id, self.tick)
        self.mode : str = "ladder_master"
        self.ready_sent : bool = False
        self.master_direction : int = 0
        self.last_call_broadcast : int = self.tick - CALL_BROADCAST_INTERVAL

    def ladder_master_step(self) -> None:
        """@brief Run one captain-side step of the incantation protocol."""
        if not self.session or self.session.master_id != self.agent_id:
            self.leave_session(notify=False)
            return
        if self.level >= 8:
            self.leave_session(notify=False)
            return
        if self.tick - self.session.started_tick > SESSION_TIMEOUT:
            self.leave_session(notify=True)
            return
        if self.food() < CAPTAIN_INCANTATION_ABORT:
            self.leave_session(notify=True)
            self.forage_food(hard=True, target=CAPTAIN_FOOD_TARGET)
            return
        if self.tick - self.last_call_broadcast >= CALL_BROADCAST_INTERVAL:
            self.broadcast("INCANTATION", self.session.session_id)
            self.last_call_broadcast = self.tick

        vision : list[dict] = self.look()
        if not vision:
            return
        tile : dict = vision[0]
        if tile.get("food", 0) > 0 and self.food() < CAPTAIN_FOOD_TARGET:
            self.take_item("food")
            return
        if tile.get("player", 1) < TEAM_TARGET_SIZE:
            return
        if len(self.session.ready) + 1 < TEAM_TARGET_SIZE:
            return
        if not self.prepare_ladder_tile(tile):
            return

        verify : list[dict] | None = self.look()
        if not verify:
            return
        tile2 : dict = verify[0]
        if tile2.get("player", 1) < TEAM_TARGET_SIZE:
            return
        if not self.tile_exact_for_level(tile2, self.level):
            return

        self.broadcast("GO", self.session.session_id)
        r : Response | None = self.incantation()
        if r and r.data_type == ResponseType.ELEVATION:
            self.incanting = True
        elif r and r.data_type == ResponseType.KO:
            self.leave_session(notify=True)

    def ladder_follower_step(self) -> None:
        """@brief Run one follower-side step of the incantation protocol."""
        if not self.session or self.session.level != self.level:
            self.leave_session(notify=False)
            return
        if self.tick - self.session.started_tick > SESSION_TIMEOUT:
            self.leave_session(notify=False)
            return
        if self.tick - self.last_master_heard > MATE_TIMEOUT:
            self.leave_session(notify=False)
            return
        if self.food() <= FOOD_CRITICAL + 3:
            self.leave_session(notify=False)
            self.forage_food(hard=True, target=SUPPORT_FOOD_TARGET)
            return

        session_id : str = self.session.session_id
        if self.master_direction == 0:
            if not self.ready_sent:
                self.broadcast("READY", session_id)
                self.ready_sent = True
            return
        if self.master_direction > 0:
            moves = self.navigator.moves_from_broadcast_direction(self.master_direction)
            self.master_direction = -1
            self.run_moves(moves)
            return
        if self.tick % INVENTORY_INTERVAL == 0:
            self.refresh_inventory()

    def prepare_ladder_tile(self, tile: dict | None = None) -> bool:
        """@brief Adjust the current tile so it exactly matches level requirements.

        @param tile Optional mutable current-tile snapshot.
        @return True when the tile exactly matches the expected stones.
        """
        if tile is None:
            vision : list[dict] | None = self.look()
            if not vision:
                return False
            tile = vision[0]
        req : dict[str, int] = self.stone_req_for_level(self.level)
        for stone in STONE_NAMES:
            extra : int = max(0, int(tile.get(stone, 0)) - req.get(stone, 0))
            for _ in range(extra):
                if self.take_item(stone):
                    tile[stone] = max(0, int(tile.get(stone, 0)) - 1)
                else:
                    break
        for stone in STONE_NAMES:
            missing = max(0, req.get(stone, 0) - int(tile.get(stone, 0)))
            for _ in range(missing):
                if int(self.inventory_data.get(stone, 0)) <= 0:
                    return False
                if self.set_item(stone):
                    tile[stone] = int(tile.get(stone, 0)) + 1
                else:
                    return False
        return self.tile_exact_for_level(tile, self.level)

    def stone_req_for_level(self, level: int) -> dict[str, int]:
        """@brief Return normalized stone requirements for a level.

        @param level Incantation level.
        @return Requirement map containing every known stone.
        """
        req : dict[str, int] = ELEVATION_TABLE.get(level, {})
        return {stone: int(req.get(stone, 0)) for stone in STONE_NAMES}

    def tile_exact_for_level(self, tile: dict, level: int) -> bool:
        """@brief Check whether a tile contains exactly the level stones.

        @param tile Current tile item counts.
        @param level Incantation level.
        @return True when every stone count equals the requirement.
        """
        req : dict[str, int] = self.stone_req_for_level(level)
        for stone in STONE_NAMES:
            if int(tile.get(stone, 0)) != req.get(stone, 0):
                return False
        return True
