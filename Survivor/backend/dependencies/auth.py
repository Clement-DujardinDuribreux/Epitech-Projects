from collections.abc import Callable

from fastapi import Depends, HTTPException, Request
from pymysql.connections import Connection

from core.security import get_token_request, get_token_subject
from dependencies.database import get_db
from services.auth import get_user


def get_current_user(request: Request, connection: Connection = Depends(get_db)) -> dict:
    user_id = get_token_subject(get_token_request(request))
    if user_id is None:
        raise HTTPException(status_code=401, detail="Invalid token")
    user = get_user(connection, user_id)
    if user is None:
        raise HTTPException(status_code=401, detail="User not found")
    return user


def require_role(role: str) -> Callable:
    def dependency(request: Request, connection: Connection = Depends(get_db)) -> dict:
        user = get_current_user(request, connection)
        if user["role"] != role:
            raise HTTPException(status_code=403, detail=f"{role.capitalize()} role required")
        return user

    return dependency


require_recruiter = require_role("recruiter")
require_admin = require_role("admin")
require_job_seeker = require_role("job_seeker")
