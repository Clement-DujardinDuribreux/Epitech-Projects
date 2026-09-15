#!/bin/sh
set -eu

HOST="localhost"
PORT=""
TEAM=""

usage()
{
    echo "USAGE: ./run_6_ai.sh -p port -n name [-h machine]"
}

while [ "$#" -gt 0 ]; do
    case "$1" in
        -p)
            [ "$#" -ge 2 ] || { usage; exit 84; }
            PORT="$2"
            shift 2
            ;;
        -n)
            [ "$#" -ge 2 ] || { usage; exit 84; }
            TEAM="$2"
            shift 2
            ;;
        -h)
            [ "$#" -ge 2 ] || { usage; exit 84; }
            HOST="$2"
            shift 2
            ;;
        *)
            usage
            exit 84
            ;;
    esac
done

[ -n "$PORT" ] && [ -n "$TEAM" ] || { usage; exit 84; }

if [ -x "./zappy_ai" ]; then
    AI="./zappy_ai"
else
    AI="python3 ai/main.py"
fi

pids=""
trap 'for pid in $pids; do kill "$pid" 2>/dev/null || true; done' INT TERM

i=1
while [ "$i" -le 6 ]; do
    $AI -p "$PORT" -n "$TEAM" -h "$HOST" &
    pids="$pids $!"
    i=$((i + 1))
done

wait
