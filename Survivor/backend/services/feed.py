import math
from pymysql.connections import Connection

from repositories import feed as feed_repo


def get_users_feed(
    connection: Connection,
    competences: list[str] | None = None,
    localisation: str | None = None,
    sector: str | None = None,
    certified: bool | None = None,
    page: int = 1,
    limit: int = 20,
) -> dict:
    if page < 1:
        page = 1
    if limit < 1:
        limit = 20

    resolved_competences = competences or []
    users = feed_repo.get_feed(
        connection=connection,
        competences=resolved_competences,
        localisation=localisation,
        sector=sector,
        certified=certified,
        page=page,
        limit=limit,
    )
    total = feed_repo.count_feed(
        connection=connection,
        competences=resolved_competences,
        localisation=localisation,
        sector=sector,
        certified=certified,
    )
    total_pages = math.ceil(total / limit) if total > 0 else 1

    return {
        "data": users,
        "total": total,
        "page": page,
        "limit": limit,
        "totalPages": total_pages,
    }


def get_user_feed(connection: Connection, user_id: str) -> dict | None:
    return feed_repo.get_user_feed(connection, user_id)


def record_view(
    connection: Connection, user_id: str, recruiter_id: str | None = None
) -> bool:
    return feed_repo.record_profile_view(connection, user_id, recruiter_id)
