# ── Stage 1: deps ──────────────────────────────────────────────────────────────
FROM python:3.12-slim AS deps

WORKDIR /app

# Install uv
COPY --from=ghcr.io/astral-sh/uv:latest /uv /usr/local/bin/uv

# Copy dependency files only (leverages layer cache)
COPY pyproject.toml uv.lock ./

# Install deps into a virtual env inside the image
RUN uv sync --frozen --no-dev --no-install-project

# ── Stage 2: runtime ───────────────────────────────────────────────────────────
FROM python:3.12-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends libmagic1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy venv from deps stage
COPY --from=deps /app/.venv /app/.venv

# Copy source code
COPY . ./

# Use the venv's Python
ENV PATH="/app/.venv/bin:$PATH"

EXPOSE 8000

CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]