#!/usr/bin/env bash
set -u

usage() {
  echo "Usage: $0 <ip> <port> [count]" >&2
}

if [[ $# -lt 2 || $# -gt 3 ]]; then
  usage
  exit 1
fi

ip="$1"
port="$2"
count="${3:-500}"

if [[ -z "$ip" || -z "$port" || ! "$count" =~ ^[1-9][0-9]*$ ]]; then
  usage
  exit 1
fi

if ! command -v curl >/dev/null 2>&1; then
  echo "Error: curl is required but was not found in PATH." >&2
  exit 1
fi

roles=("job_seeker" "recruiter")
url="http://${ip}:${port}/auth/register"
timestamp="$(date +%Y%m%d%H%M%S)"
created=0
failed=0
response_file="$(mktemp)"

cleanup() {
  rm -f "$response_file"
}
trap cleanup EXIT

random_alnum() {
  LC_ALL=C tr -dc 'A-Za-z0-9' </dev/urandom | head -c "$1"
}

for ((index = 1; index <= count; index++)); do
  suffix="$(random_alnum 8)"
  password="$(random_alnum 16)"
  role="${roles[$((RANDOM % 2))]}"
  email="user_${timestamp}_${index}_${suffix}@survivor-app.dev"
  firstname="TestFirst${index}"
  name="TestLast${index}"
  json="{\"email\":\"${email}\",\"password\":\"${password}\",\"firstname\":\"${firstname}\",\"name\":\"${name}\",\"role\":\"${role}\"}"

  : >"$response_file"
  http_code="$(curl -sS -o "$response_file" -w '%{http_code}' -X POST -H 'Content-Type: application/json' -d "$json" "$url")"
  curl_status=$?

  if [[ "$curl_status" -eq 0 && "$http_code" == "201" ]]; then
    echo "Email: ${email} / MDP: ${password} / Role: ${role}"
    created=$((created + 1))
  else
    response_body="$(tr '\n\r\t' '   ' <"$response_file" | sed 's/[[:space:]][[:space:]]*/ /g; s/^ //; s/ $//')"
    if [[ -z "$response_body" ]]; then
      response_body="curl failed with exit code ${curl_status}"
    fi
    echo "Failed: ${email} / HTTP: ${http_code} / Response: ${response_body}" >&2
    failed=$((failed + 1))
  fi
done

echo "Created: ${created} / Failed: ${failed}"
