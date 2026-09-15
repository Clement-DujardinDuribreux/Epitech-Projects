from pymysql.connections import Connection

from repositories import admin as admin_repo


def validate_media(connection: Connection, media_id: str, admin_id: str) -> dict | None:
    media = admin_repo.find_media_by_id(connection, media_id)
    if media is None:
        return None
    admin_repo.validate_media(connection, media_id, admin_id)
    return admin_repo.find_media_by_id(connection, media_id)


def reject_media(connection: Connection, media_id: str, admin_id: str, reason: str) -> bool:
    media = admin_repo.find_media_by_id(connection, media_id)
    if media is None:
        return False
    return admin_repo.reject_media(connection, media_id, admin_id, reason)


def get_pending_media(connection: Connection) -> list[dict]:
    return admin_repo.list_pending_media(connection)


def get_kpis(connection: Connection) -> dict:
    return admin_repo.get_kpis(connection)
