import os
from datetime import datetime, timedelta, timezone

import bcrypt
import jwt
from jwt import InvalidTokenError

from fastapi import HTTPException, Request

VALID_TIME = 60 * 60 * 24


def hash_password(password: str) -> str:
    salt = bcrypt.gensalt()
    return bcrypt.hashpw(password.encode("utf-8"), salt).decode("utf-8")


def verify_password(password: str, stored_password: str | bytes) -> bool:
    password_hash = (
        stored_password.encode("utf-8")
        if isinstance(stored_password, str)
        else stored_password
    )
    return bcrypt.checkpw(password.encode("utf-8"), password_hash)


def get_auth_secret() -> str:
    secret = os.getenv("AUTH_SECRET_KEY")
    if not secret:
        raise RuntimeError("AUTH_SECRET_KEY must be configured")
    return secret


def create_access_token(user_id: str) -> str:
    now = datetime.now(timezone.utc)
    payload = {
        "sub": user_id,
        "iat": now,
        "exp": now + timedelta(seconds=VALID_TIME),
    }
    return jwt.encode(payload, get_auth_secret(), algorithm="HS256")


def get_token_subject(token: str) -> str | None:
    try:
        token_payload = jwt.decode(token, get_auth_secret(), algorithms=["HS256"])
        subject = token_payload.get("sub")
        if not isinstance(subject, str) or not subject:
            return None
        return subject
    except (InvalidTokenError, RuntimeError):
        return None

def get_token_request(request: Request):
    bearer = request.headers.get("authorization")
    if bearer == None or not bearer.startswith("Bearer "):
        raise HTTPException(status_code=401, detail="Invalid Token")
    return bearer[7:]