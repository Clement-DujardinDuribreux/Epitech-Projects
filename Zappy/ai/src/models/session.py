"""@file session.py
@brief Lightweight data containers used by the ladder coordination logic.
"""

class Mate:
    """@brief Snapshot of a known teammate received through broadcasts."""

    def __init__(self, level: int, inventory: dict[str, int], tick: int):
        """@brief Create a teammate state snapshot.

        @param level Teammate level.
        @param inventory Last known teammate inventory.
        @param tick Local tick when the snapshot was received.
        """
        self.level : int = level
        self.inventory : dict[str, int] = inventory
        self.tick : int = tick


class Session:
    """@brief Shared incantation session state."""

    def __init__(self, session_id: str, level: int, master_id: int, started_tick: int, ready: set[int] | None = None):
        """@brief Create a ladder session.

        @param session_id Broadcast identifier for this session.
        @param level Incantation level targeted by the session.
        @param master_id Agent id coordinating the session.
        @param started_tick Local tick at session creation.
        @param ready Optional set of follower ids that are already ready.
        """
        self.session_id : str = session_id
        self.level : int = level
        self.master_id : int = master_id
        self.started_tick : int = started_tick
        if ready is None:
            self.ready : set[int] = set()
        else:
            self.ready : set[int] = ready
