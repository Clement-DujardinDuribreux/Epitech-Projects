from pymysql.connections import Connection


def find_media_by_id(connection: Connection, media_id: str) -> dict | None:
    with connection.cursor() as cursor:
        cursor.execute(
            "SELECT id, user_id, link, valid, validate_by FROM media WHERE id = %s",
            (media_id,),
        )
        return cursor.fetchone()


def validate_media(connection: Connection, media_id: str, admin_id: str) -> bool:
    with connection.cursor() as cursor:
        cursor.execute(
            "UPDATE media SET valid = 1, validate_by = %s, reason = NULL WHERE id = %s",
            (admin_id, media_id),
        )
        return cursor.rowcount == 1


def reject_media(connection: Connection, media_id: str, admin_id: str, reason: str) -> bool:
    with connection.cursor() as cursor:
        cursor.execute(
            "UPDATE media SET valid = 0, validate_by = %s, reason = %s WHERE id = %s",
            (admin_id, reason, media_id),
        )
        return cursor.rowcount >= 0


def list_pending_media(connection: Connection) -> list[dict]:
    query = """
        SELECT m.id, m.user_id, m.link as video_url, m.valid, m.validate_by, m.reason,
               u.firstname, u.name, u.localisation,
               COALESCE(
                   (SELECT s.sector FROM sector s WHERE s.user_id = u.id LIMIT 1),
                   'Candidat ProfilsActifs'
               ) as headline
        FROM media m
        JOIN users u ON u.id = m.user_id
        ORDER BY m.id DESC
    """
    with connection.cursor() as cursor:
        cursor.execute(query)
        return cursor.fetchall()


def get_kpis(connection: Connection) -> dict:
    with connection.cursor() as cursor:
        cursor.execute("SELECT COUNT(*) as count FROM users WHERE role = 'job_seeker'")
        total_job_seekers = cursor.fetchone().get("count", 0)

        cursor.execute("SELECT COUNT(*) as count FROM users WHERE role = 'recruiter'")
        total_recruiters = cursor.fetchone().get("count", 0)

        cursor.execute(
            "SELECT COUNT(*) as count FROM users WHERE role = 'job_seeker' AND certificated = 1"
        )
        certified_candidates = cursor.fetchone().get("count", 0)

        cursor.execute("SELECT COUNT(*) as count FROM solicitations")
        total_solicitations = cursor.fetchone().get("count", 0)

        cursor.execute("SELECT COUNT(*) as count FROM media WHERE valid = 0")
        pending_media = cursor.fetchone().get("count", 0)

    certification_rate = (
        round((certified_candidates / total_job_seekers) * 100, 1)
        if total_job_seekers > 0
        else 0.0
    )

    return {
        "activeProfiles": total_job_seekers,
        "certifiedProfiles": certified_candidates,
        "certificationRate": certification_rate,
        "totalRecruiters": total_recruiters,
        "totalInteractions": total_solicitations,
        "pendingModerationCount": pending_media,
    }
