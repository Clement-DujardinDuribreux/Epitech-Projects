from uuid import uuid4

from pymysql.connections import Connection


def find_job_seeker_by_id(connection: Connection, user_id: str) -> dict | None:
    with connection.cursor() as cursor:
        cursor.execute(
            """
            SELECT id, email, firstname, name, localisation, certificated, created_at
            FROM users
            WHERE id = %s AND role = 'job_seeker'
            """,
            (user_id,),
        )
        return cursor.fetchone()


def find_follow(connection: Connection, recruiter_id: str, job_seeker_id: str) -> dict | None:
    with connection.cursor() as cursor:
        cursor.execute(
            """
            SELECT id, user_id_recruiter, user_id_js
            FROM follow
            WHERE user_id_recruiter = %s AND user_id_js = %s
            """,
            (recruiter_id, job_seeker_id),
        )
        return cursor.fetchone()


def create_follow(connection: Connection, recruiter_id: str, job_seeker_id: str) -> dict:
    follow_id = str(uuid4())
    with connection.cursor() as cursor:
        cursor.execute(
            """
            INSERT INTO follow (id, user_id_recruiter, user_id_js)
            VALUES (%s, %s, %s)
            """,
            (follow_id, recruiter_id, job_seeker_id),
        )
    return find_follow(connection, recruiter_id, job_seeker_id)


def delete_follow(connection: Connection, recruiter_id: str, job_seeker_id: str) -> bool:
    with connection.cursor() as cursor:
        cursor.execute(
            "DELETE FROM follow WHERE user_id_recruiter = %s AND user_id_js = %s",
            (recruiter_id, job_seeker_id),
        )
        return cursor.rowcount == 1


def list_followed_users(connection: Connection, recruiter_id: str) -> list[dict]:
    with connection.cursor() as cursor:
        cursor.execute(
            """
            SELECT u.id, u.email, u.firstname, u.name, u.localisation, u.certificated, u.created_at
            FROM follow f
            INNER JOIN users u ON u.id = f.user_id_js
            WHERE f.user_id_recruiter = %s AND u.role = 'job_seeker'
            ORDER BY u.created_at DESC
            """,
            (recruiter_id,),
        )
        return cursor.fetchall()


def count_followed_users(connection: Connection, recruiter_id: str) -> int:
    with connection.cursor() as cursor:
        cursor.execute(
            """
            SELECT COUNT(*)
            FROM follow f
            INNER JOIN users u ON u.id = f.user_id_js
            WHERE f.user_id_recruiter = %s AND u.role = 'job_seeker'
            """,
            (recruiter_id,),
        )
        row = cursor.fetchone()
        return list(row.values())[0] if row else 0
