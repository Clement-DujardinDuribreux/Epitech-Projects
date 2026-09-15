"""@file foraging.py
@brief Resource collection decisions for the Zappy AI.
"""

import math
import random

from ai.src.config.constants import CAPTAIN_FOOD_TARGET, FOOD_MAX, STONE_RARITY
from ai.src.elevation import ELEVATION_TABLE, STONE_NAMES


class Foraging:
    """@brief Mixin containing food and stone collection heuristics."""

    def full_ladder_need(self) -> dict[str, int]:
        """@brief Compute all stones needed from the current level to level 8.

        @return Total missing target per stone before inventory is subtracted.
        """
        need : dict[str, int] = {}
        for stone in STONE_NAMES:
            need[stone] = 0
        for lvl in range(max(1, self.level), 8):
            req : dict[str, int] = ELEVATION_TABLE.get(lvl, {})
            for stone in STONE_NAMES:
                need[stone] += int(req.get(stone, 0))
        return need

    def bank_missing(self) -> dict[str, int]:
        """@brief Compute remaining bank stones after current inventory.

        @return Signed missing count per stone; values may be negative for surplus.
        """
        need : dict[str, int] = self.full_ladder_need()
        for stone in STONE_NAMES:
            need[stone] -= int(self.inventory_data.get(stone, 0))
        return need

    def bank_complete(self) -> bool:
        """@brief Check whether the captain has enough stones to finish the ladder.

        @return True when every bank requirement is covered.
        """
        missing : dict[str, int] = self.bank_missing()
        for stone in STONE_NAMES:
            if missing[stone] > 0:
                return False
        return True

    def bank_status(self) -> str:
        """@brief Format the remaining bank need for broadcasts or debug output.

        @return Semicolon-separated `stone=value` pairs for positive needs.
        """
        missing : dict[str, int] = self.bank_missing()
        status : str = ""
        for stone in STONE_NAMES:
            if missing[stone] > 0:
                status += f"{stone}={missing[stone]};"
        return status

    def collect_bank_stones(self) -> None:
        """@brief Pick the best immediate action to fill the captain stone bank."""
        vision : list[dict] = self.look()
        if not vision:
            self.explore()
            return
        if vision[0].get("food", 0) > 0 and self.food() < CAPTAIN_FOOD_TARGET:
            self.take_item("food")
            return
        stone : str | None = self.best_bank_stone_on_tile(vision[0])
        if stone:
            self.take_item(stone)
            return
        target : tuple[int, str] | None = self.best_visible_bank_stone_tile(vision)
        if target is not None:
            tile_index, stone = target
            self.move_to(tile_index)
            self.take_item(stone)
            return
        if self.food() < CAPTAIN_FOOD_TARGET:
            tile_index : int | None = self.best_visible_food_tile(vision)
            if tile_index is not None:
                self.move_to(tile_index)
                self.take_item("food")
                return
        self.explore()

    def best_bank_stone_on_tile(self, tile: dict) -> str | None:
        """@brief Choose the most valuable missing stone on the current tile.

        @param tile Current tile item counts.
        @return Stone name to collect, or None if no useful stone is present.
        """
        missing : dict[str, int] = self.bank_missing()
        best_stone : str | None = None
        best_score : int = 0
        for stone in STONE_NAMES:
            if tile.get(stone, 0) <= 0 or missing.get(stone, 0) <= 0:
                continue
            score = 100 + missing[stone] * STONE_RARITY[stone]
            if score > best_score:
                best_stone, best_score = stone, score
        return best_stone

    def best_visible_bank_stone_tile(self, vision: list[dict]) -> tuple[int, str] | None:
        """@brief Choose the best visible bank stone while accounting for travel cost.

        @param vision Parsed `Look` tiles.
        @return `(tile_index, stone)` for the best target, or None.
        """
        missing : dict[str, int] = self.bank_missing()
        best: tuple[int, str] | None = None
        best_score : int = 0
        for i, tile in enumerate(vision):
            cost : int = self.navigator.move_cost_to_tile(i)
            for stone in STONE_NAMES:
                count = int(tile.get(stone, 0))
                if count <= 0 or missing.get(stone, 0) <= 0:
                    continue
                score = (100 + missing[stone] * STONE_RARITY[stone]) * count - cost * 4
                if score > best_score:
                    best = (i, stone)
                    best_score = score
        return best

    def forage_food(self, hard: bool, target: int) -> None:
        """@brief Collect food from the current tile or from the best visible tile.

        @param hard When True, collect as much food as possible from the current tile.
        @param target Desired food estimate for non-hard collection.
        """
        vision : list[dict] = self.look()
        if not vision:
            self.explore()
            return
        if vision[0].get("food", 0) > 0:
            if hard:
                count = int(vision[0].get("food", 0))
            else:
                wanted = max(1, math.ceil(target - self.food()))
                count = min(int(vision[0].get("food", 0)), wanted)
            for _ in range(count):
                if self.food() >= FOOD_MAX:
                    break
                if not self.take_item("food"):
                    break
            return
        tile_index : int | None = self.best_visible_food_tile(vision)
        if tile_index is not None:
            self.move_to(tile_index)
            self.take_item("food")
            return
        self.explore()

    def support_sweep_without_stones(self) -> None:
        """@brief Let a support agent keep moving while maintaining food reserves."""
        if self.food() < CAPTAIN_FOOD_TARGET:
            self.forage_food(hard=False, target=CAPTAIN_FOOD_TARGET)
            return
        self.explore()

    def best_visible_food_tile(self, vision: list[dict]) -> int | None:
        """@brief Select the visible food tile with the best count/cost score.

        @param vision Parsed `Look` tiles.
        @return Tile index to visit, or None when no food is visible.
        """
        best_tile_index : int | None = None
        best_score : int = -(10 ** 9)
        for tile_index, tile in enumerate(vision):
            count : int = int(tile.get("food", 0))
            if count <= 0:
                continue
            cost = self.navigator.move_cost_to_tile(tile_index)
            score = count * 20 - cost * 4
            if score > best_score:
                best_tile_index, best_score = tile_index, score
        return best_tile_index

    def explore(self) -> None:
        """@brief Execute the default exploration pattern."""
        if random.randint(1, 100) <= 5:
            if random.getrandbits(1):
                self.turn_right()
            else:
                self.turn_left()
            return
        if self.explore_run > 0:
            self.explore_run -= 1
            self.move_up()
            return
        if self.explore_turn_right:
            self.run_moves(["Right", "Forward", "Right"])
        else:
            self.run_moves(["Left", "Forward", "Left"])
        self.explore_turn_right = not self.explore_turn_right
        if self.map_width == 0:
            self.map_width = 10
        self.explore_run = random.randint(max(2, self.map_width // 2), max(3, self.map_width))
