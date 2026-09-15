#!/usr/bin/env bash
# End-to-end HTTP smoke test for the ProfilsActifs API.
#
# Configuration (environment variables take precedence over the first argument):
#   API_BASE_URL=http://localhost:8000
#   ADMIN_EMAIL=admin@profilsactifs.gouv.fr
#   ADMIN_PASSWORD=AdminProfils2026!
#   SEED_MEDIA_ID=m0000000-0000-0000-0000-000000000001

set -uo pipefail

API_BASE_URL="${API_BASE_URL:-${1:-http://localhost:10000}}"
API_BASE_URL="${API_BASE_URL%/}"
ADMIN_EMAIL="${ADMIN_EMAIL:-admin@profilsactifs.gouv.fr}"
ADMIN_PASSWORD="${ADMIN_PASSWORD:-AdminProfils2026!}"
SEED_MEDIA_ID="${SEED_MEDIA_ID:-m0000000-0000-0000-0000-000000000001}"

PASSED=0
FAILED=0
REQUEST_NUMBER=0
LAST_STATUS="000"
LAST_BODY=""
TMP_DIR=""
FAKE_VIDEO=""
CANDIDATE_TOKEN=""
CANDIDATE_ID=""
RECRUITER_TOKEN=""
RECRUITER_ID=""
ADMIN_TOKEN=""
CANDIDATE_NEEDS_DELETE=0
RECRUITER_NEEDS_DELETE=0
SEED_MEDIA_NEEDS_RESTORE=0
declare -a FAILED_CASES=()

for required_command in curl jq mktemp date; do
  if ! command -v "$required_command" >/dev/null 2>&1; then
    printf 'Prerequisite missing: install %s before running this script.\n' "$required_command" >&2
    exit 2
  fi
done

TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/profilsactifs-api-test.XXXXXX")"
FAKE_VIDEO="$TMP_DIR/pitch.mp4"
printf 'ProfilsActifs integration test video\n' >"$FAKE_VIDEO"

pass_case() {
  PASSED=$((PASSED + 1))
  printf 'PASS  %s\n' "$1"
}

fail_case() {
  FAILED=$((FAILED + 1))
  FAILED_CASES+=("$1")
  printf 'FAIL  %s\n' "$1" >&2
}

request() {
  local method="$1"
  local path="$2"
  local token="${3:-}"
  local json_payload="${4:-}"
  local response_file
  local -a curl_args

  REQUEST_NUMBER=$((REQUEST_NUMBER + 1))
  response_file="$TMP_DIR/response-$REQUEST_NUMBER.json"
  curl_args=(
    --silent --show-error --connect-timeout 5 --max-time 30
    --request "$method"
    --output "$response_file"
    --write-out '%{http_code}'
    --header 'Accept: application/json'
  )

  if [[ -n "$token" ]]; then
    curl_args+=(--header "Authorization: Bearer $token")
  fi
  if [[ -n "$json_payload" ]]; then
    curl_args+=(--header 'Content-Type: application/json' --data "$json_payload")
  fi

  LAST_BODY="$response_file"
  if ! LAST_STATUS="$(curl "${curl_args[@]}" "$API_BASE_URL$path")"; then
    LAST_STATUS="000"
  fi
}

request_upload() {
  local token="$1"
  local response_file
  local -a curl_args

  REQUEST_NUMBER=$((REQUEST_NUMBER + 1))
  response_file="$TMP_DIR/response-$REQUEST_NUMBER.json"
  curl_args=(
    --silent --show-error --connect-timeout 5 --max-time 30
    --request POST
    --output "$response_file"
    --write-out '%{http_code}'
    --header 'Accept: application/json'
    --header "Authorization: Bearer $token"
    --form "file=@$FAKE_VIDEO;type=video/mp4"
  )

  LAST_BODY="$response_file"
  if ! LAST_STATUS="$(curl "${curl_args[@]}" "$API_BASE_URL/job_seeker/video/upload")"; then
    LAST_STATUS="000"
  fi
}

expect_status() {
  local label="$1"
  local expected_status="$2"
  if [[ "$LAST_STATUS" == "$expected_status" ]]; then
    pass_case "$label (HTTP $expected_status)"
  else
    fail_case "$label (expected HTTP $expected_status, got $LAST_STATUS)"
  fi
}

expect_json() {
  local label="$1"
  local filter="$2"
  if jq -e "$filter" "$LAST_BODY" >/dev/null 2>&1; then
    pass_case "$label"
  else
    fail_case "$label (unexpected JSON response)"
  fi
}

cleanup_request() {
  local method="$1"
  local path="$2"
  local token="$3"
  curl --silent --show-error --connect-timeout 3 --max-time 10 \
    --request "$method" \
    --header "Authorization: Bearer $token" \
    --output /dev/null \
    "$API_BASE_URL$path" >/dev/null 2>&1 || true
}

cleanup() {
  if [[ "$SEED_MEDIA_NEEDS_RESTORE" -eq 1 && -n "$ADMIN_TOKEN" ]]; then
    printf 'Cleanup: restoring seed media %s.\n' "$SEED_MEDIA_ID" >&2
    cleanup_request PATCH "/admin/media/valid/$SEED_MEDIA_ID" "$ADMIN_TOKEN"
  fi
  if [[ "$CANDIDATE_NEEDS_DELETE" -eq 1 && -n "$CANDIDATE_TOKEN" ]]; then
    printf 'Cleanup: deleting temporary candidate.\n' >&2
    cleanup_request DELETE /auth/delete "$CANDIDATE_TOKEN"
  fi
  if [[ "$RECRUITER_NEEDS_DELETE" -eq 1 && -n "$RECRUITER_TOKEN" ]]; then
    printf 'Cleanup: deleting temporary recruiter.\n' >&2
    cleanup_request DELETE /auth/delete "$RECRUITER_TOKEN"
  fi
  [[ -z "$TMP_DIR" ]] || rm -rf "$TMP_DIR"
}

on_exit() {
  local original_status=$?
  trap - EXIT INT TERM
  cleanup
  printf '\nSummary: %d passed, %d failed.\n' "$PASSED" "$FAILED"
  if ((FAILED > 0)); then
    printf 'Failed cases:\n' >&2
    printf '  - %s\n' "${FAILED_CASES[@]}" >&2
  fi
  if ((original_status != 0 || FAILED > 0)); then
    exit 1
  fi
}

trap on_exit EXIT
trap 'exit 130' INT TERM

printf 'Testing API at %s\n' "$API_BASE_URL"

# Preconditions: do not create or change data until the API and database are ready.
request GET /
expect_status 'GET /' 200
expect_json 'GET / returns the API message' '.message == "API ProfilsActifs"'

request GET /health
expect_status 'GET /health' 200
expect_json 'GET /health confirms database readiness' '.status == "ok"'
if [[ "$LAST_STATUS" != '200' ]] || ! jq -e '.status == "ok"' "$LAST_BODY" >/dev/null 2>&1; then
  printf 'API precondition failed: start the API and database, then wait for %s/health to return {"status":"ok"}.\n' "$API_BASE_URL" >&2
  exit 1
fi

SUFFIX="$(date +%s)-$$-$RANDOM"
CANDIDATE_EMAIL="api-test-candidate-$SUFFIX@abc.com"
RECRUITER_EMAIL="api-test-recruiter-$SUFFIX@abc.com"
TEST_PASSWORD="ApiTestPassword!42"
export ADMIN_EMAIL CANDIDATE_EMAIL RECRUITER_EMAIL SEED_MEDIA_ID

candidate_registration="$(jq -cn \
  --arg email "$CANDIDATE_EMAIL" \
  '{email: $email, password: "ApiTestPassword!42", firstname: "Api", name: "Candidate", role: "job_seeker", localisation: "Testville", birthdate: "1990-01-01"}')"
recruiter_registration="$(jq -cn \
  --arg email "$RECRUITER_EMAIL" \
  '{email: $email, password: "ApiTestPassword!42", firstname: "Api", name: "Recruiter", role: "recruiter", localisation: "Testville", birthdate: "1988-01-01"}')"

# Authentication and temporary identities.
request POST /auth/register '' "$candidate_registration"
expect_status 'POST /auth/register creates candidate' 201
expect_json 'Candidate registration returns a token, public identity and role' '(.access_token | type == "string" and length > 0) and .user.email == $ENV.CANDIDATE_EMAIL and .user.role == "job_seeker" and (.user | has("password") | not)'
CANDIDATE_TOKEN="$(jq -r '.access_token // empty' "$LAST_BODY" 2>/dev/null || true)"
CANDIDATE_ID="$(jq -r '.user.id // empty' "$LAST_BODY" 2>/dev/null || true)"
export CANDIDATE_ID
if [[ -n "$CANDIDATE_TOKEN" && -n "$CANDIDATE_ID" && "$LAST_STATUS" == '201' ]]; then CANDIDATE_NEEDS_DELETE=1; fi

request POST /auth/register '' "$recruiter_registration"
expect_status 'POST /auth/register creates recruiter' 201
expect_json 'Recruiter registration returns a token, public identity and role' '(.access_token | type == "string" and length > 0) and .user.email == $ENV.RECRUITER_EMAIL and .user.role == "recruiter" and (.user | has("password") | not)'
RECRUITER_TOKEN="$(jq -r '.access_token // empty' "$LAST_BODY" 2>/dev/null || true)"
RECRUITER_ID="$(jq -r '.user.id // empty' "$LAST_BODY" 2>/dev/null || true)"
export RECRUITER_ID
if [[ -n "$RECRUITER_TOKEN" && -n "$RECRUITER_ID" && "$LAST_STATUS" == '201' ]]; then RECRUITER_NEEDS_DELETE=1; fi

wrong_login="$(jq -cn --arg email "$CANDIDATE_EMAIL" '{email: $email, password: "wrong-password"}')"
request POST /auth/login '' "$wrong_login"
expect_status 'POST /auth/login rejects an invalid password' 401

request POST /auth/register '' "$candidate_registration"
expect_status 'POST /auth/register rejects a duplicate email' 409

request GET /job_seeker/profile
expect_status 'GET /job_seeker/profile rejects an anonymous request' 401

request GET /job_seeker/profile "$RECRUITER_TOKEN"
expect_status 'GET /job_seeker/profile rejects a recruiter token' 403

candidate_login="$(jq -cn --arg email "$CANDIDATE_EMAIL" '{email: $email, password: "ApiTestPassword!42"}')"
request POST /auth/login '' "$candidate_login"
expect_status 'POST /auth/login authenticates candidate' 200
expect_json 'Candidate login returns a token and public candidate' '(.access_token | type == "string" and length > 0) and .user.id == $ENV.CANDIDATE_ID and (.user | has("password") | not)'

request GET /auth/me "$CANDIDATE_TOKEN"
expect_status 'GET /auth/me' 200
expect_json 'GET /auth/me returns the candidate public identity' '.id == $ENV.CANDIDATE_ID and .email == $ENV.CANDIDATE_EMAIL and .role == "job_seeker" and (has("password") | not)'

request GET /auth/me/age-status "$CANDIDATE_TOKEN"
expect_status 'GET /auth/me/age-status' 200
expect_json 'GET /auth/me/age-status confirms the candidate is eligible' '.valid == true'

request PATCH /auth/me/birthdate "$CANDIDATE_TOKEN" '{"birthdate":"1991-01-01"}'
expect_status 'PATCH /auth/me/birthdate' 200
expect_json 'PATCH /auth/me/birthdate keeps a public adult profile' '.id == $ENV.CANDIDATE_ID and .birthdate == "1991-01-01" and (has("password") | not)'

# Candidate profile and media routes.
request GET /job_seeker/profile "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/profile' 200
expect_json 'Candidate profile is readable' '.id == $ENV.CANDIDATE_ID and .role == "job_seeker"'

request GET /job_seeker/competences "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/competences' 200
expect_json 'Candidate competences start as a JSON list' '.competences | type == "array"'

request GET /job_seeker/sectors "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/sectors' 200
expect_json 'Candidate sectors start as a JSON list' '.sectors | type == "array"'

request GET /job_seeker/video "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/video has no video before creation' 404

request PUT /job_seeker/profile "$CANDIDATE_TOKEN" '{"firstname":"Api","name":"Candidate Updated","localisation":"Testville","birthdate":"1990-01-01","certificated":false}'
expect_status 'PUT /job_seeker/profile' 200
expect_json 'PUT /job_seeker/profile persists the submitted values' '.id == $ENV.CANDIDATE_ID and .name == "Candidate Updated" and .localisation == "Testville"'

request PUT /job_seeker/competences "$CANDIDATE_TOKEN" '{"add":["API Testing"],"remove":[]}'
expect_status 'PUT /job_seeker/competences' 200
expect_json 'PUT /job_seeker/competences persists the new competence' '.competences | index("API Testing") != null'

request PUT /job_seeker/sectors "$CANDIDATE_TOKEN" '{"add":["Technology"],"remove":[]}'
expect_status 'PUT /job_seeker/sectors' 200
expect_json 'PUT /job_seeker/sectors persists the new sector' '.sectors | index("Technology") != null'

request PUT /job_seeker/video "$CANDIDATE_TOKEN" '{"link":"https://example.test/api-test-pitch.mp4"}'
expect_status 'PUT /job_seeker/video' 200
expect_json 'PUT /job_seeker/video returns a valid media object' '.link == "https://example.test/api-test-pitch.mp4" and .valid == true'

request GET /job_seeker/video "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/video returns the configured link' 200
expect_json 'GET /job_seeker/video reads the configured link' '.link == "https://example.test/api-test-pitch.mp4" and (.valid == true or .valid == 1)'

request_upload "$CANDIDATE_TOKEN"
expect_status 'POST /job_seeker/video/upload' 200
expect_json 'POST /job_seeker/video/upload creates the current uploaded video' '(.link | startswith("/uploads/video_")) and .valid == true'

request GET /job_seeker/video "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/video returns the uploaded video' 200
expect_json 'Uploaded video is now the current video' '(.link | startswith("/uploads/video_")) and (.valid == true or .valid == 1)'

request POST /job_seeker/certification "$CANDIDATE_TOKEN" '{"score":16,"total":20}'
expect_status 'POST /job_seeker/certification' 200
expect_json 'POST /job_seeker/certification validates a passing score' '.certificated == true and .is_passed == true and .score == 16 and .total == 20'

request GET /job_seeker/interactions "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/interactions before recruiter activity' 200
expect_json 'Candidate interactions exposes views and solicitations' '(.viewsCount | type == "number") and (.solicitations | type == "array")'

# Feed and view tracking.
request GET '/feed/users?page=1&limit=10&competences=API%20Testing&localisation=Testville&sector=Technology&certified=true'
expect_status 'GET /feed/users with filters and pagination' 200
expect_json 'Filtered feed has its pagination envelope and temporary candidate' '(.data | type == "array") and .total >= 1 and .page == 1 and .limit == 10 and .totalPages >= 1 and any(.data[]; .id == $ENV.CANDIDATE_ID)'

request GET "/feed/user/$CANDIDATE_ID"
expect_status 'GET /feed/user/{candidateId}' 200
expect_json 'GET /feed/user/{candidateId} returns candidate relations' '.id == $ENV.CANDIDATE_ID and (.competences | index("API Testing") != null) and (.sectors | index("Technology") != null)'

request GET /feed/user/00000000-0000-0000-0000-000000000000
expect_status 'GET /feed/user/{missingId} returns 404' 404

request POST "/feed/user/$CANDIDATE_ID/view" "$RECRUITER_TOKEN"
expect_status 'POST /feed/user/{candidateId}/view' 204

request GET /job_seeker/interactions "$CANDIDATE_TOKEN"
expect_status 'GET /job_seeker/interactions after a view' 200
expect_json 'Profile view is counted in candidate interactions' '.viewsCount >= 1 and (.solicitations | type == "array")'

# Recruiter follows and solicitations.
request POST "/profile/follow/$CANDIDATE_ID" "$RECRUITER_TOKEN"
expect_status 'POST /profile/follow/{candidateId}' 200
expect_json 'Follow response links the temporary recruiter and candidate' '.user_id_recruiter == $ENV.RECRUITER_ID and .user_id_js == $ENV.CANDIDATE_ID'

request GET /profile/follow "$RECRUITER_TOKEN"
expect_status 'GET /profile/follow' 200
expect_json 'GET /profile/follow contains the temporary candidate' '(.data | type == "array") and .total >= 1 and any(.data[]; .id == $ENV.CANDIDATE_ID)'

request GET /profile/follow "$CANDIDATE_TOKEN"
expect_status 'GET /profile/follow rejects a candidate token' 403

request DELETE "/profile/unfollow/$CANDIDATE_ID" "$RECRUITER_TOKEN"
expect_status 'DELETE /profile/unfollow/{candidateId}' 204

solicitation_payload="$(jq -cn --arg candidate_id "$CANDIDATE_ID" '{candidate_id: $candidate_id, message: "Integration test solicitation"}')"
request POST /recruiter/solicitations "$RECRUITER_TOKEN" "$solicitation_payload"
expect_status 'POST /recruiter/solicitations' 201
expect_json 'Solicitation belongs to the recruiter and targets the candidate' '.recruiter_id == $ENV.RECRUITER_ID and .job_seeker_id == $ENV.CANDIDATE_ID and .status == "sent"'
SOLICITATION_ID="$(jq -r '.id // empty' "$LAST_BODY" 2>/dev/null || true)"
export SOLICITATION_ID

request GET /recruiter/solicitations "$RECRUITER_TOKEN"
expect_status 'GET /recruiter/solicitations' 200
expect_json 'GET /recruiter/solicitations contains the new solicitation' 'type == "array" and any(.[]; .id == $ENV.SOLICITATION_ID and .recruiter_id == $ENV.RECRUITER_ID and .job_seeker_id == $ENV.CANDIDATE_ID and .status == "sent")'

request PATCH "/recruiter/solicitations/$SOLICITATION_ID/status" "$RECRUITER_TOKEN" '{"status":"interview"}'
expect_status 'PATCH /recruiter/solicitations/{id}/status' 200
expect_json 'Solicitation status changes to interview' '.id == $ENV.SOLICITATION_ID and .recruiter_id == $ENV.RECRUITER_ID and .job_seeker_id == $ENV.CANDIDATE_ID and .status == "interview"'

request DELETE "/recruiter/solicitations/$SOLICITATION_ID" "$RECRUITER_TOKEN"
expect_status 'DELETE /recruiter/solicitations/{id}' 204

# Administration. The selected seed media is restored even on interruption or failure.
admin_login="$(jq -cn --arg email "$ADMIN_EMAIL" --arg password "$ADMIN_PASSWORD" '{email: $email, password: $password}')"
request POST /auth/login '' "$admin_login"
expect_status 'POST /auth/login authenticates the configured administrator' 200
expect_json 'Administrator login returns an admin identity' '(.access_token | type == "string" and length > 0) and .user.email == $ENV.ADMIN_EMAIL and .user.role == "admin"'
ADMIN_TOKEN="$(jq -r '.access_token // empty' "$LAST_BODY" 2>/dev/null || true)"

request GET /admin/kpis "$ADMIN_TOKEN"
expect_status 'GET /admin/kpis' 200
expect_json 'Admin KPIs expose all six counters' 'has("activeProfiles") and has("certifiedProfiles") and has("certificationRate") and has("totalRecruiters") and has("totalInteractions") and has("pendingModerationCount")'

request GET /admin/media/pending "$ADMIN_TOKEN"
expect_status 'GET /admin/media/pending before moderation' 200
expect_json 'GET /admin/media/pending returns a JSON list' 'type == "array"'

SEED_MEDIA_NEEDS_RESTORE=1
request PATCH "/admin/media/reject/$SEED_MEDIA_ID" "$ADMIN_TOKEN"
expect_status 'PATCH /admin/media/reject/{seedMediaId}' 200
expect_json 'Seed media rejection reports the selected identifier' '.status == "rejected" and .id == $ENV.SEED_MEDIA_ID'

request GET /admin/media/pending "$ADMIN_TOKEN"
expect_status 'GET /admin/media/pending after rejection' 200
expect_json 'Rejected seed media appears in the pending list' 'type == "array" and any(.[]; .id == $ENV.SEED_MEDIA_ID and (.valid == false or .valid == 0))'

request PATCH "/admin/media/valid/$SEED_MEDIA_ID" "$ADMIN_TOKEN"
expect_status 'PATCH /admin/media/valid/{seedMediaId}' 200
expect_json 'Seed media is restored as valid' '.id == $ENV.SEED_MEDIA_ID and (.valid == true or .valid == 1)'
if [[ "$LAST_STATUS" == '200' ]] && jq -e '.id == $ENV.SEED_MEDIA_ID and (.valid == true or .valid == 1)' "$LAST_BODY" >/dev/null 2>&1; then
  SEED_MEDIA_NEEDS_RESTORE=0
fi

# Explicit deletions are verified; the EXIT trap remains a fallback for failures.
request DELETE /auth/delete "$CANDIDATE_TOKEN"
expect_status 'DELETE /auth/delete removes temporary candidate' 204
if [[ "$LAST_STATUS" == '204' ]]; then CANDIDATE_NEEDS_DELETE=0; fi

request DELETE /auth/delete "$RECRUITER_TOKEN"
expect_status 'DELETE /auth/delete removes temporary recruiter' 204
if [[ "$LAST_STATUS" == '204' ]]; then RECRUITER_NEEDS_DELETE=0; fi
