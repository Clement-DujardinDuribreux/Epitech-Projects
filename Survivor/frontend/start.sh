#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

PORT="${PORT:-5173}"

echo "=========================================================="
echo " Démarrage de l'application ProfilsActifs"
echo " Accès : http://localhost:${PORT}"
echo "=========================================================="

if command -v python3 >/dev/null 2>&1; then
  exec python3 -m http.server "$PORT"
elif command -v npx >/dev/null 2>&1; then
  exec npx serve -p "$PORT" .
elif command -v python >/dev/null 2>&1; then
  exec python -m SimpleHTTPServer "$PORT"
elif command -v php >/dev/null 2>&1; then
  exec php -S "localhost:${PORT}"
else
  echo "Erreur : Aucun serveur HTTP trouvé (veuillez installer python3, node/npx ou php)."
  exit 1
fi

