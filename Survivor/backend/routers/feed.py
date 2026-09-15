from fastapi import APIRouter, Depends, HTTPException, Query, Request
from pymysql.connections import Connection

from core.security import get_token_request, get_token_subject
from dependencies.database import get_db
from services.feed import get_user_feed, get_users_feed, record_view

router = APIRouter(prefix="/feed", tags=["feed"])


@router.get("/users")
def feed_users(
    connection: Connection = Depends(get_db),
    page: int = 1,
    limit: int = 20,
    competences: list[str] = Query(default=[]),
    localisation: str | None = None,
    sector: str | None = None,
    certified: bool | None = None,
    nb_users: int | None = None,
):
    effective_limit = nb_users if (nb_users is not None and nb_users > 0) else limit
    return get_users_feed(
        connection=connection,
        competences=competences,
        localisation=localisation,
        sector=sector,
        certified=certified,
        page=page,
        limit=effective_limit,
    )


@router.get("/user/{user_id}")
def feed_user(user_id: str, connection: Connection = Depends(get_db)):
    user = get_user_feed(connection, user_id)
    if user is None:
        raise HTTPException(status_code=404, detail="Job seeker not found")
    return user


@router.post("/user/{user_id}/view", status_code=204)
def view_user(user_id: str, request: Request, connection: Connection = Depends(get_db)):
    recruiter_id = None
    try:
        token = get_token_request(request)
        recruiter_id = get_token_subject(token)
    except Exception:
        pass

    record_view(connection, user_id, recruiter_id)
