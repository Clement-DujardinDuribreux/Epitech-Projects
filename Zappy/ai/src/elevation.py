"""@file elevation.py
@brief Incantation requirements and helpers for Zappy levels.
"""

STONE_NAMES = ("linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame")

ELEVATION_TABLE = {
    1: {"players": 1, "linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0},
    2: {"players": 2, "linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0},
    3: {"players": 2, "linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0},
    4: {"players": 4, "linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0},
    5: {"players": 4, "linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0},
    6: {"players": 6, "linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0},
    7: {"players": 6, "linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1},
}

class Elevation:
    """@brief Provides requirement checks for a single player level."""

    def __init__(self, level: int):
        """@brief Store the elevation requirements associated with a level.

        @param level Current player level, usually from 1 to 7.
        """
        self.level : int = level
        self.requirements : dict[str, int] = ELEVATION_TABLE.get(level, {})

    def has_enough_stones(self, inventory: dict[str, int]) -> bool:
        """@brief Check whether an inventory contains every required stone.

        @param inventory Current inventory values indexed by resource name.
        @return True when all stone requirements for this level are satisfied.
        """
        for stone, required_amount in self.stone_requirements().items():
            if inventory.get(stone, 0) < required_amount:
                return False
        return True

    def has_any_required_stone(self, inventory: dict[str, int]) -> bool:
        """@brief Check whether an inventory already contains a useful stone.

        @param inventory Current inventory values indexed by resource name.
        @return True when at least one required stone is present.
        """
        for stone, required_amount in self.stone_requirements().items():
            if required_amount > 0 and inventory.get(stone, 0) > 0:
                return True
        return False

    def missing_stones(self, inventory: dict[str, int]) -> dict[str, int]:
        """@brief Compute the missing stones for this level.

        @param inventory Current inventory values indexed by resource name.
        @return Dictionary containing only stones whose current amount is too low.
        """
        missing : dict[str, int] = {}
        for stone, required_amount in self.stone_requirements().items():
            current_amount : int = inventory.get(stone, 0)
            if current_amount < required_amount:
                missing[stone] = required_amount - current_amount
        return missing

    def stone_requirements(self) -> dict[str, int]:
        """@brief Return normalized stone requirements for this level.

        @return Requirement map containing every known stone name.
        """
        requirements : dict[str, int] = {}
        for stone in STONE_NAMES:
            requirements[stone] = self.requirements.get(stone, 0)
        return requirements

    def players_needed(self, level: int) -> int:
        """@brief Return the number of players needed for an incantation level.

        @param level Level to inspect.
        @return Required player count, or 0 for an unknown level.
        """
        if level not in ELEVATION_TABLE:
            return 0
        return ELEVATION_TABLE[level]["players"]

    def stones_to_set(self, level: int) -> list[str]:
        """@brief Expand level requirements into individual `Set` operations.

        @param level Level to inspect.
        @return Stone names repeated once per required item.
        """
        if level not in ELEVATION_TABLE:
            return []
        requirements : dict[str, int] = ELEVATION_TABLE[level]
        stones : list[str] = []
        for stone in STONE_NAMES:
            for _ in range(requirements.get(stone, 0)):
                stones.append(stone)
        return stones
