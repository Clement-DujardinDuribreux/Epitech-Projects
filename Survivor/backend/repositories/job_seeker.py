from uuid import uuid4
from pymysql.connections import Connection


def find_profile(connection: Connection, user_id: str) -> dict | None:
    query = """
        SELECT id, email, firstname, name, role, localisation, certificated,
               birthdate, headline, bio, phone, avatar_url, created_at, last_update_at
        FROM users
        WHERE id = %s AND role = 'job_seeker'
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (user_id,))
        return cursor.fetchone()


def find_competences(connection: Connection, user_id: str) -> list[str]:
    return find_choices(connection, user_id, "competences", "competence")


def find_sectors(connection: Connection, user_id: str) -> list[str]:
    return find_choices(connection, user_id, "sector", "sector")


def find_video(connection: Connection, user_id: str) -> dict | None:
    query = """
        SELECT link, valid, reason
        FROM media
        WHERE user_id = %s
        ORDER BY id DESC
        LIMIT 1
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (user_id,))
        return cursor.fetchone()


ALLOWED_PROFILE_FIELDS = {
    "firstname": "firstname",
    "name": "name",
    "localisation": "localisation",
    "birthdate": "birthdate",
    "headline": "headline",
    "bio": "bio",
    "phone": "phone",
    "avatar_url": "avatar_url",
}

ALLOWED_CHOICES = {
    "competences": "competence",
    "sector": "sector",
}


def update_profile(connection: Connection, user_id: str, values: dict) -> bool:
    valid_keys = [k for k in values.keys() if k in ALLOWED_PROFILE_FIELDS]
    if not valid_keys:
        return False

    clauses = [ALLOWED_PROFILE_FIELDS[k] + " = %s" for k in valid_keys]
    assignments = ", ".join(clauses)
    query = (
        "UPDATE users SET "
        + assignments
        + ", last_update_at = CURRENT_TIMESTAMP WHERE id = %s AND role = 'job_seeker'"
    )
    params = tuple([values[k] for k in valid_keys] + [user_id])
    with connection.cursor() as cursor:
        cursor.execute(query, params)
        return cursor.rowcount != 0


def set_certification(connection: Connection, user_id: str, certificated: bool) -> bool:
    query = "UPDATE users SET certificated = %s, last_update_at = CURRENT_TIMESTAMP WHERE id = %s AND role = 'job_seeker'"
    with connection.cursor() as cursor:
        cursor.execute(query, (1 if certificated else 0, user_id))
        return cursor.rowcount != 0


def replace_competences(connection: Connection, user_id: str, competences: list[str]) -> list[str]:
    return replace_choices(connection, user_id, competences, "competences", "competence")


def replace_sectors(connection: Connection, user_id: str, sectors: list[str]) -> list[str]:
    return replace_choices(connection, user_id, sectors, "sector", "sector")


def replace_video(connection: Connection, user_id: str, link: str, valid: bool = True) -> dict:
    with connection.cursor() as cursor:
        cursor.execute(
            "UPDATE media SET valid = 0 WHERE user_id = %s AND valid = 1",
            (user_id,),
        )
        cursor.execute(
            """
            INSERT INTO media (id, user_id, link, valid)
            VALUES (%s, %s, %s, %s)
            """,
            (str(uuid4()), user_id, link, 1 if valid else 0),
        )
    return {"link": link, "valid": valid}


def find_choices(connection: Connection, user_id: str, table: str, column: str) -> list[str]:
    if table not in ALLOWED_CHOICES or ALLOWED_CHOICES[table] != column:
        raise ValueError(f"Table or column not allowed: {table}.{column}")

    if table == "competences":
        query = "SELECT competence FROM competences WHERE user_id = %s"
    else:
        query = "SELECT sector FROM sector WHERE user_id = %s"

    with connection.cursor() as cursor:
        cursor.execute(query, (user_id,))
        return [row[column] for row in cursor.fetchall()]


def replace_choices(connection: Connection, user_id: str, choices: list[str], table: str, column: str) -> list[str]:
    if table not in ALLOWED_CHOICES or ALLOWED_CHOICES[table] != column:
        raise ValueError(f"Table or column not allowed: {table}.{column}")

    with connection.cursor() as cursor:
        if table == "competences":
            cursor.execute("DELETE FROM competences WHERE user_id = %s", (user_id,))
            for choice in sorted(choices):
                cursor.execute(
                    "INSERT INTO competences (id, user_id, competence) VALUES (%s, %s, %s)",
                    (str(uuid4()), user_id, choice),
                )
        else:
            cursor.execute("DELETE FROM sector WHERE user_id = %s", (user_id,))
            for choice in sorted(choices):
                cursor.execute(
                    "INSERT INTO sector (id, user_id, sector) VALUES (%s, %s, %s)",
                    (str(uuid4()), user_id, choice),
                )
    return sorted(choices)


def get_interactions(connection: Connection, user_id: str) -> dict:
    with connection.cursor() as cursor:
        # 1. Total des vues de profil
        cursor.execute(
            "SELECT COUNT(*) as count FROM profile_views WHERE job_seeker_id = %s",
            (user_id,),
        )
        views_count = cursor.fetchone().get("count", 0)

        # 2. Sollicitations reçues
        cursor.execute(
            """
            SELECT s.id, s.recruiter_id, s.message, s.status, s.created_at,
                   s.is_read, s.is_archived, s.reply, s.replied_at,
                   u.firstname as recruiter_firstname, u.name as recruiter_name,
                   u.email as recruiter_email, u.localisation as recruiter_location
            FROM solicitations s
            JOIN users u ON u.id = s.recruiter_id
            WHERE s.job_seeker_id = %s
            ORDER BY s.created_at DESC
            """,
            (user_id,),
        )
        solicitations = cursor.fetchall()

    return {
        "viewsCount": views_count,
        "solicitations": solicitations,
    }


def find_solicitation_for_job_seeker(
    connection: Connection, user_id: str, solicitation_id: str
) -> dict | None:
    query = """
        SELECT s.id, s.recruiter_id, s.message, s.status, s.created_at,
               s.is_read, s.is_archived, s.reply, s.replied_at,
               u.firstname as recruiter_firstname, u.name as recruiter_name,
               u.email as recruiter_email, u.localisation as recruiter_location
        FROM solicitations s
        JOIN users u ON u.id = s.recruiter_id
        WHERE s.id = %s AND s.job_seeker_id = %s
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (solicitation_id, user_id))
        return cursor.fetchone()


def set_solicitation_read(
    connection: Connection, user_id: str, solicitation_id: str, is_read: bool
) -> bool:
    query = "UPDATE solicitations SET is_read = %s WHERE id = %s AND job_seeker_id = %s"
    with connection.cursor() as cursor:
        cursor.execute(query, (1 if is_read else 0, solicitation_id, user_id))
        return cursor.rowcount != 0


def set_solicitation_archived(
    connection: Connection, user_id: str, solicitation_id: str, is_archived: bool
) -> bool:
    query = "UPDATE solicitations SET is_archived = %s WHERE id = %s AND job_seeker_id = %s"
    with connection.cursor() as cursor:
        cursor.execute(query, (1 if is_archived else 0, solicitation_id, user_id))
        return cursor.rowcount != 0


def reply_to_solicitation(
    connection: Connection, user_id: str, solicitation_id: str, message: str
) -> bool:
    query = """
        UPDATE solicitations
        SET reply = %s, replied_at = CURRENT_TIMESTAMP, is_read = 1
        WHERE id = %s AND job_seeker_id = %s
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (message, solicitation_id, user_id))
        return cursor.rowcount != 0
