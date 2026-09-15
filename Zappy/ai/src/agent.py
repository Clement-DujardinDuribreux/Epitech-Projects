"""! @file agent.py
@brief Main autonomous Zappy AI agent.
"""

import random
import time

from ai.src.clients import ServerResponse as Response, DataType as ResponseType, Client as ZappyClient
from ai.src.action.foraging import Foraging
from ai.src.action.ladder import Ladder
from ai.src.action.population import Population
from ai.src.config.constants import (
    CALL_BROADCAST_INTERVAL,
    CAPTAIN_COLLECT_FOOD_MIN,
    CAPTAIN_FOOD_TARGET,
    CAPTAIN_INCANTATION_MIN,
    COMMAND_COST,
    FOOD_CRITICAL,
    FOOD_TIME_UNITS,
    INVENTORY_INTERVAL,
    INV_BROADCAST_INTERVAL,
    MATE_TIMEOUT,
    SUPPORT_FOOD_TARGET,
    SUPPORT_INCANTATION_MIN,
)
from ai.src.elevation import STONE_NAMES
from ai.src.navigation import Navigation
from ai.src.models.session import Mate, Session
from ai.src.utils.crypto import decrypt_message, encrypt_message


class Agent(Foraging, Ladder, Population):
    """@brief Runtime state machine combining networking, foraging, ladder, and population logic."""

    def __init__(self, ip: str, port: int, team_name: str):
        """@brief Create an agent and initialize local decision state.

        @param ip Zappy server hostname or IP address.
        @param port Zappy server TCP port.
        @param team_name Team name used during the handshake.
        """
        self.client : ZappyClient = ZappyClient(ip, port)
        self.team_name : str = team_name
        self.agent_id : int = random.randint(1000, 9999)
        self.level : int = 1
        self.map_width : int = 0
        self.map_height : int = 0
        self.running : bool = True
        self.inventory_data: dict[str, int] = {"food": 10}
        for stone in STONE_NAMES:
            self.inventory_data[stone] = 0
        self.food_estimate : float = 10.0
        self.navigator : Navigation = Navigation()
        self.tick: int = 0
        self.mode: str = "free"
        self.session: Session | None = None
        self.master_direction: int = -1
        self.last_master_heard: int = 0
        self.ready_sent: bool = False
        self.incanting: bool = False
        self.mates: dict[int, Mate] = {}
        self.last_inv_broadcast: int = -999
        self.last_call_broadcast: int = -999
        self.last_population_check: int = -999
        self.last_fork_tick: int = -999
        self.free_slots: int = 0
        self.population_enabled: bool = True
        self.explore_run: int = random.randint(2, 8)
        self.explore_turn_right: bool = random.choice([True, False])

    def connect(self) -> None:
        """@brief Connect to the server, read the map size, and publish initial inventory."""
        slots, width, height = self.client.initialize(self.team_name)
        self.free_slots: int = max(0, slots)
        self.map_width: int = width
        self.map_height: int = height
        self.explore_run: int = random.randint(max(2, width // 3), max(3, width))
        self.refresh_inventory()
        self.broadcast_inventory(force=True)

    def run(self) -> None:
        """@brief Execute the main agent loop until death or shutdown."""
        while self.running:
            self.tick += 1
            self.drain_events()
            if not self.running:
                break
            if self.incanting:
                time.sleep(0.01)
                continue
            if self.tick % INVENTORY_INTERVAL == 0:
                self.refresh_inventory()
            if self.tick - self.last_inv_broadcast >= INV_BROADCAST_INTERVAL:
                self.broadcast_inventory()
            if self.level >= 8:
                self.idle_winner()
                continue
            self.decide()

    def decide(self) -> None:
        """@brief Select the next high-level behavior for the current tick."""
        food : float = self.food()
        if food <= FOOD_CRITICAL:
            self.leave_session(notify=False)
            self.forage_food(hard=True, target=max(SUPPORT_FOOD_TARGET, CAPTAIN_COLLECT_FOOD_MIN))
            return
        if self.mode == "ladder_master":
            self.ladder_master_step()
            return
        if self.mode == "ladder_follower":
            self.ladder_follower_step()
            return
        if self.is_captain():
            self.captain_free_step()
        else:
            self.support_free_step()

    def captain_free_step(self) -> None:
        """@brief Decide the next action when this free agent is captain."""
        if self.population_step():
            return
        if self.food() < CAPTAIN_COLLECT_FOOD_MIN:
            self.forage_food(hard=False, target=CAPTAIN_FOOD_TARGET)
            return
        if not self.bank_complete():
            self.collect_bank_stones()
            return
        if self.food() < CAPTAIN_INCANTATION_MIN:
            self.forage_food(hard=False, target=CAPTAIN_FOOD_TARGET)
            return
        if not self.team_ready_for_ladder():
            if self.population_step():
                return
            if self.food() < CAPTAIN_FOOD_TARGET:
                self.forage_food(hard=False, target=CAPTAIN_FOOD_TARGET)
            else:
                self.support_sweep_without_stones()
            return
        self.start_ladder_session()

    def support_free_step(self) -> None:
        """@brief Decide the next action when this free agent is not captain."""
        self.forage_food(hard=False, target=SUPPORT_FOOD_TARGET)

    def move_to(self, tile_index: int) -> None:
        """@brief Move to a tile visible in the last `Look` result.

        @param tile_index Target tile index.
        """
        self.run_moves(self.navigator.moves_to_tile(tile_index))

    def run_moves(self, moves: list[str]) -> None:
        """@brief Execute movement commands in batches respecting pending limits.

        @param moves Ordered movement commands.
        """
        if not moves:
            return
        i : int = 0
        while i < len(moves) and self.running:
            chunk : list[str] = moves[i: i + self.client.maxPending]
            for command in chunk:
                self.spend_for_command(command)
            responses : list[Response] = self.client.commandBatch(chunk)
            for event in self.client.popEvents():
                self.handle_event(event)
            for r in responses:
                self.handle_response(r)
            i += len(chunk)

    def look(self) -> list[dict] | None:
        """@brief Run `Look` and return parsed vision data when available."""
        response : Response | None = self.look_around()
        if response and response.data_type == ResponseType.LOOK:
            return response.data
        return None

    def food(self) -> float:
        """@brief Return the current non-negative food estimate."""
        return max(0.0, self.food_estimate)

    def spend_for_command(self, command: str) -> None:
        """@brief Decrease the local food estimate according to command cost.

        @param command Command that is about to be sent.
        """
        verb: str = command.split(" ", 1)[0]
        time_units: int = COMMAND_COST.get(verb, 7)
        if time_units <= 0:
            return
        self.food_estimate: float = max(0.0, self.food_estimate - time_units / FOOD_TIME_UNITS)
        self.inventory_data["food"] = min(int(self.inventory_data.get("food", 0)), int(self.food_estimate))

    def add_food(self, amount: int = 1) -> None:
        """@brief Increase local food counters after a successful take.

        @param amount Food units collected.
        """
        self.food_estimate += amount
        self.inventory_data["food"] = int(self.food_estimate)

    def refresh_inventory(self) -> None:
        """@brief Synchronize local inventory from the server."""
        response : Response | None = self.inventory()
        if response and response.data_type == ResponseType.INVENTORY and isinstance(response.data, dict):
            for stone in STONE_NAMES:
                response.data.setdefault(stone, 0)
            response.data.setdefault("food", self.inventory_data.get("food", 0))
            self.inventory_data.update(response.data)
            self.food_estimate = float(self.inventory_data.get("food", 0))

    def broadcast_inventory(self, force: bool = False) -> None:
        """@brief Broadcast the current inventory to teammates.

        @param force Ignore the normal broadcast interval when True.
        """
        if not force and self.tick - self.last_inv_broadcast < INV_BROADCAST_INTERVAL:
            return
        self.last_inv_broadcast: int = self.tick
        chunks: list[str] = [f"food={int(self.food())}"]
        for stone in STONE_NAMES:
            chunks.append(f"{stone}={int(self.inventory_data.get(stone, 0))}")
        self.broadcast("INV", ";".join(chunks))

    def new_mates(self) -> dict[int, Mate]:
        """@brief Return known teammates after dropping timed-out entries."""
        for mate_id, mate in list(self.mates.items()):
            if self.tick - mate.tick >= MATE_TIMEOUT:
                del self.mates[mate_id]
        return self.mates

    def known_team_size(self) -> int:
        """@brief Return confirmed team size including self."""
        return 1 + len(self.new_mates())

    def projected_team_size(self) -> int:
        """@brief Return the known team size."""
        return self.known_team_size()

    def move_up(self) -> Response | None:
        """@brief Send `Forward`."""
        return self.cmd("Forward")

    def turn_right(self) -> Response | None:
        """@brief Send `Right`."""
        return self.cmd("Right")

    def turn_left(self) -> Response | None:
        """@brief Send `Left`."""
        return self.cmd("Left")

    def look_around(self) -> Response | None:
        """@brief Send `Look`."""
        return self.cmd("Look")

    def inventory(self) -> Response | None:
        """@brief Send `Inventory`."""
        return self.cmd("Inventory")

    def broadcast_raw(self, message: str) -> Response | None:
        """@brief Send an encrypted raw broadcast payload.

        @param message Plain payload before encryption.
        """
        return self.cmd(f"Broadcast {encrypt_message(message)}")

    def connect_nbr(self) -> Response | None:
        """@brief Send `Connect_nbr`."""
        return self.cmd("Connect_nbr")

    def fork(self) -> Response | None:
        """@brief Send `Fork`."""
        return self.cmd("Fork")

    def eject(self) -> Response | None:
        """@brief Send `Eject`."""
        return self.cmd("Eject")

    def take(self, item: str) -> Response | None:
        """@brief Send `Take item`.

        @param item Resource name.
        """
        return self.cmd(f"Take {item}")

    def set_down(self, item: str) -> Response | None:
        """@brief Send `Set item`.

        @param item Resource name.
        """
        return self.cmd(f"Set {item}")

    def incantation(self) -> Response | None:
        """@brief Send `Incantation`."""
        return self.cmd("Incantation")

    def moveUp(self) -> Response | None:
        """@brief Compatibility wrapper for `move_up`."""
        return self.move_up()

    def turnRight(self) -> Response | None:
        """@brief Compatibility wrapper for `turn_right`."""
        return self.turn_right()

    def turnLeft(self) -> Response | None:
        """@brief Compatibility wrapper for `turn_left`."""
        return self.turn_left()

    def lookAround(self) -> Response | None:
        """@brief Compatibility wrapper for `look_around`."""
        return self.look_around()

    def setDown(self, item: str) -> Response | None:
        """@brief Compatibility wrapper for `set_down`.

        @param item Resource name.
        """
        return self.set_down(item)

    def cmd(self, command: str) -> Response | None:
        """@brief Send one command and process resulting events.

        @param command Zappy command.
        @return Parsed command response.
        """
        self.spend_for_command(command)
        response: Response | None = self.client.command(command)
        for event in self.client.popEvents():
            self.handle_event(event)
        if response:
            self.handle_response(response)
        return response

    def take_item(self, item: str) -> bool:
        """@brief Take an item and update local inventory on success.

        @param item Resource name.
        @return True when the server accepted the action.
        """
        response: Response | None = self.take(item)
        if response and response.data_type == ResponseType.OK:
            if item == "food":
                self.add_food()
            else:
                self.inventory_data[item] = int(self.inventory_data.get(item, 0)) + 1
            return True
        return False

    def set_item(self, item: str) -> bool:
        """@brief Set an item down and update local inventory on success.

        @param item Resource name.
        @return True when the server accepted the action.
        """
        response: Response | None = self.set_down(item)
        if response and response.data_type == ResponseType.OK:
            self.inventory_data[item] = max(0, int(self.inventory_data.get(item, 0)) - 1)
            if item == "food":
                self.food_estimate = min(self.food_estimate, float(self.inventory_data[item]))
            return True
        return False

    def broadcast(self, message: str, payload: str | None = None) -> Response | None:
        """@brief Broadcast either a raw message or a structured team message.

        @param message Action name or raw payload.
        @param payload Optional structured payload.
        @return Parsed command response.
        """
        if payload is None:
            return self.broadcast_raw(message)
        return self.broadcast_raw(
            f"ZPY|{self.team_name}|{self.agent_id}|{self.level}|{message}|{payload}"
        )

    def drain_events(self) -> None:
        """@brief Poll and handle asynchronous server events."""
        for event in self.client.poll():
            self.handle_event(event)

    def handle_response(self, response: Response) -> None:
        """@brief Apply state changes caused by a command response.

        @param response Parsed server response.
        """
        if response.data_type == ResponseType.DEAD:
            self.running = False
            return
        if response.data_type == ResponseType.ELEVATION:
            self.incanting = True
            return
        if response.data_type == ResponseType.LEVEL:
            old_level = self.level
            self.level = int(response.data)
            self.incanting = False
            self.refresh_inventory()
            self.broadcast_inventory(force=True)
            self.after_level_up(old_level)
            return
        if response.data_type == ResponseType.KO and self.incanting:
            self.incanting = False
            self.leave_session(notify=False)

    def after_level_up(self, old_level: int) -> None:
        """@brief Update session state after a successful level-up.

        @param old_level Level before the server confirmation.
        """
        if self.level >= 8:
            self.leave_session(notify=False)
            return
        if self.session and self.mode in ("ladder_master", "ladder_follower"):
            self.session.level = self.level
            self.session.started_tick = self.tick
            if self.mode == "ladder_master":
                self.session.ready.clear()
                self.last_call_broadcast = self.tick - CALL_BROADCAST_INTERVAL
            else:
                self.ready_sent = False
                self.master_direction = -1
                self.last_master_heard = self.tick
            return
        self.leave_session(notify=False)

    def handle_event(self, event: Response) -> None:
        """@brief Dispatch asynchronous server events.

        @param event Parsed event response.
        """
        if event.data_type in (ResponseType.DEAD, ResponseType.LEVEL, ResponseType.ELEVATION):
            self.handle_response(event)
            return
        if event.data_type == ResponseType.KO and self.incanting:
            self.handle_response(event)
            return
        if event.data_type == ResponseType.EJECT:
            self.leave_session(notify=False)
            return
        if event.data_type == ResponseType.BROADCAST and isinstance(event.data, dict):
            self.handle_broadcast(event.data.get("direction", -1), event.data.get("text", ""))

    def handle_broadcast(self, direction: int, text: str) -> None:
        """@brief Decode and process a teammate broadcast.

        @param direction Direction reported by the server.
        @param text Encrypted broadcast payload.
        """
        text: str = decrypt_message(text) or ""
        parts: list[str] = text.split("|", 5)
        if len(parts) != 6 or parts[0] != "ZPY":
            return
        _, team, agent_id_str, level_str, action, payload = parts
        if team != self.team_name or not agent_id_str.isdigit() or not level_str.isdigit():
            return
        sender: int = int(agent_id_str)
        msg_level: int = int(level_str)
        if sender == self.agent_id:
            return
        if action == "INV":
            inv: dict[str, int] = {}
            for chunk in payload.split(";"):
                if "=" not in chunk:
                    continue
                key, value = chunk.split("=", 1)
                if not value.lstrip("-").isdigit():
                    continue
                inv[key] = int(value)
            inv.setdefault("food", 0)
            for stone in STONE_NAMES:
                inv.setdefault(stone, 0)
            self.mates[sender] = Mate(msg_level, inv, self.tick)
            return

        if action == "INCANTATION":
            if msg_level != self.level:
                return
            if self.mode == "ladder_master":
                return
            if sender != self.captain_id() and not (self.session and sender == self.session.master_id):
                return
            if self.food() < SUPPORT_INCANTATION_MIN and self.mode != "ladder_follower":
                return
            if self.session and self.session.session_id == payload:
                self.master_direction: int = int(direction)
                self.last_master_heard: int = self.tick
                if direction != 0:
                    self.ready_sent: bool = False
                return
            self.mode: str = "ladder_follower"
            self.session: Session | None = Session(payload, self.level, sender, self.tick)
            self.master_direction: int = int(direction)
            self.last_master_heard: int = self.tick
            self.ready_sent: bool = False
            return

        if action == "READY":
            if self.mode == "ladder_master" and self.session and payload == self.session.session_id:
                self.session.ready.add(sender)
            return

        if action == "CANCEL":
            if self.session and payload == self.session.session_id and sender == self.session.master_id:
                self.leave_session(notify=False)
            return

    def leave_session(self, notify: bool = True) -> None:
        """@brief Reset ladder session state.

        @param notify Broadcast cancellation when leaving as ladder master.
        """
        if notify and self.mode == "ladder_master" and self.session:
            self.broadcast("CANCEL", self.session.session_id)
        self.mode: str = "free"
        self.session: Session | None = None
        self.master_direction: int = -1
        self.last_master_heard: int = 0
        self.ready_sent: bool = False
        self.incanting: bool = False

    def idle_winner(self) -> None:
        """@brief Idle behavior once the agent reached the maximum level."""
        if self.tick % 40 == 0:
            self.broadcast("MAX", "8")
        time.sleep(0.05)
