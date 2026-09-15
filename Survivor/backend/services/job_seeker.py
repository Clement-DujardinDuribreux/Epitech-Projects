import os
from pathlib import Path
from uuid import uuid4
from pymysql.connections import Connection
import magic

from repositories import job_seeker as job_seeker_repo
from schemas.job_seeker import (
    CertificationValidationRequest,
    JobSeekerCompetencesUpdate,
    JobSeekerProfileUpdate,
    JobSeekerSectorsUpdate,
    JobSeekerVideoUpdate,
)


def get_profile(connection: Connection, user_id: str) -> dict | None:
    return job_seeker_repo.find_profile(connection, user_id)


def get_competences(connection: Connection, user_id: str) -> list[str]:
    return job_seeker_repo.find_competences(connection, user_id)


def get_sectors(connection: Connection, user_id: str) -> list[str]:
    return job_seeker_repo.find_sectors(connection, user_id)


def get_video(connection: Connection, user_id: str) -> dict | None:
    return job_seeker_repo.find_video(connection, user_id)


def update_profile(
    connection: Connection, user_id: str, payload: JobSeekerProfileUpdate
) -> dict | None:
    values = payload.model_dump(exclude_unset=True)
    if values:
        if not job_seeker_repo.update_profile(connection, user_id, values):
            return None
    return job_seeker_repo.find_profile(connection, user_id)


def validate_certification(
    connection: Connection, user_id: str, payload: CertificationValidationRequest
) -> dict:
    is_passed = (payload.score / payload.total) >= 0.8
    if is_passed:
        job_seeker_repo.set_certification(connection, user_id, True)
    return {
        "certificated": is_passed,
        "score": payload.score,
        "total": payload.total,
        "is_passed": is_passed,
    }


def _update_choices(
    connection: Connection,
    user_id: str,
    items_to_add: list[str],
    items_to_remove: list[str],
    choice_config: tuple[str, str],
) -> list[str]:
    table, column = choice_config
    current = set(job_seeker_repo.find_choices(connection, user_id, table, column))
    updated = (current - set(items_to_remove)) | set(items_to_add)
    if len(updated) > 5:
        raise ValueError(f"Un maximum de 5 {table} est autorisé")

    return job_seeker_repo.replace_choices(connection, user_id, list(updated), table, column)


def update_competences(
    connection: Connection, user_id: str, payload: JobSeekerCompetencesUpdate
) -> list[str]:
    return _update_choices(
        connection,
        user_id,
        payload.add,
        payload.remove,
        ("competences", "competence"),
    )


def update_sectors(
    connection: Connection, user_id: str, payload: JobSeekerSectorsUpdate
) -> list[str]:
    return _update_choices(
        connection,
        user_id,
        payload.add,
        payload.remove,
        ("sector", "sector"),
    )


def update_video(
    connection: Connection, user_id: str, payload: JobSeekerVideoUpdate
) -> dict | None:
    return job_seeker_repo.replace_video(connection, user_id, str(payload.link))

def check_magic_bytes(content: bytes) -> bool:
    mime_type = magic.from_buffer(content, mime=True)
    if (mime_type == "video/mp4"  or mime_type == "video/webm" or mime_type == "video/ogg"):
        return True
    return False

def save_uploaded_video(
    connection: Connection, user_id: str, filename: str, content: bytes
) -> dict:
    # Sauvegarde sur disque dans /app/uploads/ (ou ./uploads/)
    upload_dir = Path("/app/uploads")
    if not upload_dir.exists():
        upload_dir = Path("./uploads")
    upload_dir.mkdir(parents=True, exist_ok=True)
    if not check_magic_bytes(content):
        print(f"Le fichier n'est pas un fichier vidéo valide. Type MIME détecté: {magic.from_buffer(content, mime=True)}")
        raise ValueError("Le fichier n'est pas un fichier vidéo valide.")

    extension = Path(filename).suffix.lower() or ".mp4"
    saved_filename = f"video_{user_id}_{uuid4().hex[:8]}{extension}"
    target_path = upload_dir / saved_filename
    target_path.write_bytes(content)

    video_url = f"/uploads/{saved_filename}"
    return job_seeker_repo.replace_video(connection, user_id, video_url)


def save_uploaded_avatar(
    connection: Connection, user_id: str, filename: str, content: bytes
) -> dict:
    upload_dir = Path("/app/uploads")
    if not upload_dir.exists():
        upload_dir = Path("./uploads")
    upload_dir.mkdir(parents=True, exist_ok=True)

    extension = Path(filename).suffix.lower() or ".jpg"
    saved_filename = f"avatar_{user_id}_{uuid4().hex[:8]}{extension}"
    target_path = upload_dir / saved_filename
    target_path.write_bytes(content)

    avatar_url = f"/uploads/{saved_filename}"
    job_seeker_repo.update_profile(connection, user_id, {"avatar_url": avatar_url})
    return {"avatar_url": avatar_url}


def get_interactions(connection: Connection, user_id: str) -> dict:
    return job_seeker_repo.get_interactions(connection, user_id)


def set_solicitation_read(
    connection: Connection, user_id: str, solicitation_id: str, is_read: bool
) -> dict | None:
    if not job_seeker_repo.set_solicitation_read(connection, user_id, solicitation_id, is_read):
        return None
    return job_seeker_repo.find_solicitation_for_job_seeker(connection, user_id, solicitation_id)


def set_solicitation_archived(
    connection: Connection, user_id: str, solicitation_id: str, is_archived: bool
) -> dict | None:
    if not job_seeker_repo.set_solicitation_archived(connection, user_id, solicitation_id, is_archived):
        return None
    return job_seeker_repo.find_solicitation_for_job_seeker(connection, user_id, solicitation_id)


def reply_to_solicitation(
    connection: Connection, user_id: str, solicitation_id: str, message: str
) -> dict | None:
    if not job_seeker_repo.reply_to_solicitation(connection, user_id, solicitation_id, message):
        return None
    return job_seeker_repo.find_solicitation_for_job_seeker(connection, user_id, solicitation_id)
