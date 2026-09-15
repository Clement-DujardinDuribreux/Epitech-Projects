"""@file clients.py
@brief Non-blocking TCP client and response parser for the Zappy server protocol.
"""

from enum import Enum
import select
import socket
import time

class DataType(Enum):
    """@brief Typed categories for parsed server messages."""

    OK = 0
    KO = 1
    LOOK = 2
    INVENTORY = 3
    DEAD = 4
    ELEVATION = 5
    LEVEL = 6
    BROADCAST = 7
    EJECT = 8
    VALUE = 9
    WELCOME = 10

class ServerResponse:
    """@brief Parsed server response with a type and optional payload."""

    def __init__(self, dataType: DataType, data: str):
        """@brief Store a parsed server response.

        @param dataType Response category.
        @param data Parsed payload associated with the category.
        """
        self.data_type: DataType = dataType
        self.data: str = data

class Client:
    """@brief Zappy TCP client with command queue accounting."""

    def __init__(self, ip: str, port: int) -> None:
        """@brief Create a client targeting a server endpoint.

        @param ip Server hostname or IP address.
        @param port Server TCP port.
        """
        self.ip: str = ip
        self.port: int = port
        self.socket: socket.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setblocking(False)
        self.buffer: str = ""
        self.pendingCount: int = 0
        self.maxPending: int = 10
        self.events: list[ServerResponse] = []

    def connect(self) -> bool:
        """@brief Open the TCP connection.

        @return True when the socket connected successfully.
        """
        try:
            self.socket.setblocking(True)
            self.socket.connect((self.ip, self.port))
            self.socket.setblocking(False)
            return True
        except Exception:
            return False

    def receiveAvailable(self, block: bool = False) -> None:
        """@brief Read currently available bytes into the line buffer.

        @param block When True, wait briefly until at least one response is available.
        """
        if block:
            timeout: float = 0.25
        else:
            timeout: float = 0.0
        while True:
            ready, _, _ = select.select([self.socket], [], [], timeout)
            if self.socket not in ready:
                if block:
                    time.sleep(0.002)
                return
            try:
                data: bytes | None = self.socket.recv(4096)
            except (BlockingIOError, InterruptedError):
                return
            except (ConnectionResetError):
                self.buffer += "dead\n"
                return
            if not data:
                self.buffer += "dead\n"
                return
            self.buffer += data.decode("utf-8", errors="ignore")
            if not block:
                return
            timeout = 0.0

    def receiveLine(self) -> str:
        """@brief Read one complete server line.

        @return A stripped line without the trailing newline.
        """
        while "\n" not in self.buffer:
            self.receiveAvailable(block=True)
        line, self.buffer = self.buffer.split("\n", 1)
        return line.strip()

    def initialize(self, teamName: str) -> tuple[int, int, int]:
        """@brief Run the Zappy welcome/team handshake.

        @param teamName Team name sent to the server.
        @return Tuple `(available_slots, map_width, map_height)`.
        @throws Exception when the handshake is rejected or malformed.
        """
        if not self.connect():
            raise Exception("Failed to connect to server.")
        welcome: str = self.receiveLine()
        if welcome != "WELCOME":
            raise Exception(f"Unexpected initial message: {welcome}")
        self.socket.sendall((teamName + "\n").encode())
        slotCount: str = self.receiveLine()
        while slotCount.startswith(("message ", "eject:")):
            self.events.append(self.parseLine(slotCount))
            slotCount = self.receiveLine()
        if slotCount.strip() == "ko":
            raise Exception("Team name rejected by server.")
        if not slotCount.strip().isdigit():
            raise Exception(f"Invalid team initialization response: {slotCount}")
        clientNumber: int = int(slotCount.strip())
        mapSize: str = self.receiveLine()
        while mapSize.startswith(("message ", "eject:")):
            self.events.append(self.parseLine(mapSize))
            mapSize = self.receiveLine()
        sizeParts: list[str] = mapSize.strip().split()
        if len(sizeParts) != 2 or not all(p.isdigit() for p in sizeParts):
            raise Exception(f"Invalid map size format: {mapSize}")
        return (clientNumber, int(sizeParts[0]), int(sizeParts[1]))

    def canSend(self) -> bool:
        """@brief Check whether the command queue has free capacity.

        @return True when another command can be sent immediately.
        """
        return self.pendingCount < self.maxPending

    def sendRawData(self, command: str) -> bool:
        """@brief Send one raw command line to the server.

        @param command Command without trailing newline.
        @return True if the command was accepted into the pending queue.
        """
        if self.canSend():
            try:
                self.socket.sendall((command + "\n").encode())
                self.pendingCount += 1
                return True
            except OSError:
                self.buffer += "dead\n"
        return False

    def command(self, command: str) -> ServerResponse:
        """@brief Send one command and wait for its command response.

        @param command Zappy command line.
        @return Parsed command response.
        """
        if not self.sendRawData(command):
            return ServerResponse(DataType.KO, None)
        return self.readCommandResponse()

    def commandBatch(self, commands: list[str]) -> list[ServerResponse]:
        """@brief Send up to `maxPending` commands and read their responses.

        @param commands Ordered commands to send.
        @return Responses in server order.
        """
        responses: list[ServerResponse] = []
        sentCount: int = 0
        for cmd in commands[:self.maxPending]:
            if self.sendRawData(cmd):
                sentCount += 1
        for _ in range(sentCount):
            response: ServerResponse = self.readCommandResponse()
            responses.append(response)
            if response.data_type == DataType.DEAD:
                break
        return responses

    def readCommandResponse(self) -> ServerResponse:
        """@brief Read the next response belonging to a command.

        Broadcast and eject messages are stored as asynchronous events.
        @return The next non-event response.
        """
        while True:
            response: ServerResponse = self.parseLine(self.receiveLine())
            if response.data_type in (DataType.BROADCAST, DataType.EJECT):
                self.events.append(response)
                continue
            if response.data_type == DataType.DEAD:
                self.events.append(response)
            return response

    def popEvents(self) -> list[ServerResponse]:
        """@brief Return and clear queued asynchronous events.

        @return Broadcast, eject, or death events collected during command reads.
        """
        events: list[ServerResponse] = self.events
        self.events: list[ServerResponse] = []
        return events

    def poll(self) -> list[ServerResponse]:
        """@brief Parse every complete line currently available on the socket.

        @return Parsed responses and events.
        """
        responses: list[ServerResponse] = []
        self.receiveAvailable()
        while "\n" in self.buffer:
            line, self.buffer = self.buffer.split("\n", 1)
            line = line.strip()
            if line:
                responses.append(self.parseLine(line))
        return responses

    def isInventory(self, parts: list[str]) -> bool:
        """@brief Detect whether bracketed server content is an inventory.

        @param parts Comma-separated bracket content.
        @return True when every item matches `name amount`.
        """
        names: set[str] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"}
        if not parts:
            return False
        for part in parts:
            tokens: list[str] = part.strip().split()
            if len(tokens) != 2 or tokens[0] not in names or not tokens[1].isdigit():
                return False
        return True

    def parseLine(self, line: str) -> ServerResponse:
        """@brief Parse one raw protocol line.

        @param line Raw line without trailing newline.
        @return Typed response with parsed payload when possible.
        """
        if line == "dead":
            return ServerResponse(DataType.DEAD, None)
        if line == "ok":
            self.pendingCount = max(0, self.pendingCount - 1)
            return ServerResponse(DataType.OK, None)
        if line == "ko":
            self.pendingCount = max(0, self.pendingCount - 1)
            return ServerResponse(DataType.KO, None)
        if line == "Elevation underway":
            return ServerResponse(DataType.ELEVATION, None)
        if line.startswith("Current level:"):
            self.pendingCount = max(0, self.pendingCount - 1)
            return ServerResponse(DataType.LEVEL, line.split(":", 1)[1].strip())
        if line.startswith("message "):
            payload : str = line[len("message "):]
            if "," not in payload:
                return ServerResponse(DataType.VALUE, line)
            directionStr, text = payload.split(",", 1)
            try:
                direction : int = int(directionStr.strip())
            except ValueError:
                return ServerResponse(DataType.VALUE, line)
            return ServerResponse(DataType.BROADCAST, {"direction": direction, "text": text.strip()})
        if line.startswith("eject:"):
            try:
                direction = int(line.split(":", 1)[1].strip())
            except ValueError:
                direction = None
            return ServerResponse(DataType.EJECT, direction)
        if line == "WELCOME":
            return ServerResponse(DataType.WELCOME, None)
        if line.startswith("[") and line.endswith("]"):
            content : str = line[1:-1].strip()
            if not content:
                self.pendingCount = max(0, self.pendingCount - 1)
                return ServerResponse(DataType.LOOK, [])
            parts : list[str] = [p.strip() for p in content.split(",")]
            self.pendingCount = max(0, self.pendingCount - 1)
            if self.isInventory(parts):
                return ServerResponse(DataType.INVENTORY, self.parseInventory(content))
            return ServerResponse(DataType.LOOK, self.parseLook(content))
        if line.isdigit():
            self.pendingCount = max(0, self.pendingCount - 1)
            return ServerResponse(DataType.VALUE, int(line))
        return ServerResponse(DataType.VALUE, line)

    def parseLook(self, content: str) -> list[dict]:
        """@brief Parse the content of a `Look` response.

        @param content Bracket content without the outer square brackets.
        @return One dictionary per visible tile with item counts.
        """
        result: list[dict] = []
        for cell in content.split(","):
            cell : str = cell.strip()
            cellDict: dict = {}
            for item in cell.split():
                cellDict[item] = cellDict.get(item, 0) + 1
            result.append(cellDict)
        return result

    def parseInventory(self, content: str) -> dict:
        """@brief Parse the content of an `Inventory` response.

        @param content Bracket content without the outer square brackets.
        @return Inventory counts indexed by item name.
        """
        inventory: dict = {}
        for item in content.split(","):
            parts: list[str] = item.strip().split()
            if len(parts) == 2:
                try:
                    inventory[parts[0]] = int(parts[1])
                except ValueError:
                    continue
        return inventory
