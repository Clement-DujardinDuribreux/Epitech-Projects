from fastapi import APIRouter, Depends, HTTPException
from pymysql.connections import Connection

from dependencies.auth import require_recruiter
from dependencies.database import get_db
from services.profile import (
    FollowNotFoundError,
    FollowTargetNotFoundError,
    follow_user,
    get_followed_users,
    unfollow_user,
)

router = APIRouter(prefix="/profile", tags=["profile"])


@router.post("/follow/{user_id}")
def follow_profile(user_id: str, current_user: dict = Depends(require_recruiter), connection: Connection = Depends(get_db)):
    try:
        return follow_user(connection, current_user["id"], user_id)
    except FollowTargetNotFoundError:
        raise HTTPException(status_code=404, detail="Job seeker not found")


@router.delete("/unfollow/{user_id}", status_code=204)
def unfollow_profile(user_id: str, current_user: dict = Depends(require_recruiter), connection: Connection = Depends(get_db)):
    try:
        unfollow_user(connection, current_user["id"], user_id)
    except FollowNotFoundError:
        raise HTTPException(status_code=404, detail="Follow not found")


@router.get("/follow")
def list_followed_profiles(current_user: dict = Depends(require_recruiter), connection: Connection = Depends(get_db)):
    return get_followed_users(connection, current_user["id"])
