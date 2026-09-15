from uuid import uuid4
from pymysql.connections import Connection


def create_solicitation(
    connection: Connection, recruiter_id: str, candidate_id: str, message: str
) -> dict:
    solicitation_id = str(uuid4())
    query = """
        INSERT INTO solicitations (id, recruiter_id, job_seeker_id, message, status)
        VALUES (%s, %s, %s, %s, 'sent')
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (solicitation_id, recruiter_id, candidate_id, message))

    return find_solicitation_by_id(connection, solicitation_id)


def find_solicitation_by_id(connection: Connection, solicitation_id: str) -> dict | None:
    query = """
        SELECT s.id, s.recruiter_id, s.job_seeker_id, s.message, s.status,
               s.created_at, s.updated_at,
               u.firstname as candidate_firstname, u.name as candidate_name,
               u.email as candidate_email, u.localisation as candidate_location
        FROM solicitations s
        JOIN users u ON u.id = s.job_seeker_id
        WHERE s.id = %s
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (solicitation_id,))
        return cursor.fetchone()


def list_solicitations_by_recruiter(
    connection: Connection, recruiter_id: str
) -> list[dict]:
    query = """
        SELECT s.id, s.recruiter_id, s.job_seeker_id, s.message, s.status,
               s.created_at, s.updated_at,
               u.firstname as candidate_firstname, u.name as candidate_name,
               u.email as candidate_email, u.localisation as candidate_location
        FROM solicitations s
        JOIN users u ON u.id = s.job_seeker_id
        WHERE s.recruiter_id = %s
        ORDER BY s.created_at DESC
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (recruiter_id,))
        return cursor.fetchall()


def update_solicitation_status(
    connection: Connection, solicitation_id: str, recruiter_id: str, status: str
) -> bool:
    query = """
        UPDATE solicitations
        SET status = %s
        WHERE id = %s AND recruiter_id = %s
    """
    with connection.cursor() as cursor:
        cursor.execute(query, (status, solicitation_id, recruiter_id))
        return cursor.rowcount > 0


def delete_solicitation(
    connection: Connection, solicitation_id: str, recruiter_id: str
) -> bool:
    query = "DELETE FROM solicitations WHERE id = %s AND recruiter_id = %s"
    with connection.cursor() as cursor:
        cursor.execute(query, (solicitation_id, recruiter_id))
        return cursor.rowcount > 0
