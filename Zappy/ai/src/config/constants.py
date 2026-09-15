"""@file constants.py
@brief Tunable timings, thresholds, and resource weights for the Python AI.
"""

COMMAND_COST = {
    "Forward": 7,
    "Right": 7,
    "Left": 7,
    "Look": 7,
    "Inventory": 1,
    "Broadcast": 7,
    "Connect_nbr": 0,
    "Fork": 42,
    "Eject": 7,
    "Take": 7,
    "Set": 7,
    "Incantation": 300,
}

FOOD_TIME_UNITS = 126
TEAM_TARGET_SIZE = 6
MATE_TIMEOUT = 520
INVENTORY_INTERVAL = 10
INV_BROADCAST_INTERVAL = 14
CALL_BROADCAST_INTERVAL = 3
POPULATION_CHECK_INTERVAL = 10
FORK_COOLDOWN_TICKS = 60
SESSION_TIMEOUT = 1800
FOOD_CRITICAL = 10
SUPPORT_FOOD_TARGET = 55
SUPPORT_INCANTATION_MIN = 42
CAPTAIN_FOOD_TARGET = 90
CAPTAIN_COLLECT_FOOD_MIN = 28
CAPTAIN_INCANTATION_MIN = 65
CAPTAIN_INCANTATION_ABORT = 35
FOOD_MAX = 140

STONE_RARITY = {
    "linemate": 3,
    "deraumere": 5,
    "sibur": 7,
    "mendiane": 9,
    "phiras": 12,
    "thystame": 18,
}

BROADCAST_KEY = "D0_Y04_7H1NK_17_1S_3A2Y_70_D3CYPH3R_TH3_Z4PPY_M4573R_67"
