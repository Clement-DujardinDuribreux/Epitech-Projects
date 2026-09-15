from fastapi import APIRouter, Depends, File, HTTPException, UploadFile
from pymysql.connections import Connection

from core.age import MinimumAgeError
from dependencies.auth import require_job_seeker
from dependencies.database import get_db
from schemas.job_seeker import (
    CertificationValidationRequest,
    JobSeekerCompetencesUpdate,
    JobSeekerProfileUpdate,
    JobSeekerSectorsUpdate,
    JobSeekerVideoUpdate,
    SolicitationReplyRequest,
)
from services import job_seeker as job_seeker_service


router = APIRouter(prefix="/job_seeker", tags=["job_seeker"])


@router.get("/profile")
def get_profile(current_job_seeker: dict = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    profile = job_seeker_service.get_profile(
        connection, str(current_job_seeker["id"])
    )
    if profile is None:
        raise HTTPException(status_code=404, detail="Job seeker not found")
    return profile


@router.get("/competences")
def get_competences(current_job_seeker: dict = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    competences = job_seeker_service.get_competences(
        connection, str(current_job_seeker["id"])
    )
    return {"competences": competences}


@router.get("/sectors")
def get_sectors(current_job_seeker: dict[str, object] = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    sectors = job_seeker_service.get_sectors(
        connection, str(current_job_seeker["id"])
    )
    return {"sectors": sectors}


@router.get("/video")
def get_video(current_job_seeker: dict = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    video = job_seeker_service.get_video(
        connection, str(current_job_seeker["id"])
    )
    if video is None:
        raise HTTPException(status_code=404, detail="Video not found")
    return video


@router.put("/profile")
def update_profile(payload: JobSeekerProfileUpdate, current_job_seeker: dict = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    try:
        profile = job_seeker_service.update_profile(
            connection, str(current_job_seeker["id"]), payload
        )
    except MinimumAgeError as error:
        raise HTTPException(
            status_code=422, detail="You must be at least 16 years old"
        ) from error
    if profile is None:
        raise HTTPException(status_code=404, detail="Job seeker not found")
    return profile


@router.put("/competences")
def update_competences(payload: JobSeekerCompetencesUpdate, current_job_seeker: dict = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    try:
        competences = job_seeker_service.update_competences(
            connection, str(current_job_seeker["id"]), payload
        )
    except ValueError as error:
        raise HTTPException(status_code=422, detail=str(error)) from error
    return {"competences": competences}


@router.put("/sectors")
def update_sectors(payload: JobSeekerSectorsUpdate, current_job_seeker: dict = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    try:
        sectors = job_seeker_service.update_sectors(
            connection, str(current_job_seeker["id"]), payload
        )
    except ValueError as error:
        raise HTTPException(status_code=422, detail=str(error)) from error
    return {"sectors": sectors}


@router.put("/video")
def update_video(payload: JobSeekerVideoUpdate, current_job_seeker: dict = Depends(require_job_seeker), connection: Connection = Depends(get_db)):
    return job_seeker_service.update_video(
        connection, str(current_job_seeker["id"]), payload
    )


@router.post("/certification")
def validate_certification(
    payload: CertificationValidationRequest,
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    return job_seeker_service.validate_certification(
        connection, str(current_job_seeker["id"]), payload
    )


@router.post("/video/upload")
async def upload_video(
    file: UploadFile = File(...),
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    MAX_SIZE = 100 * 1024 * 1024  # 100 Mo max
    content = await file.read()
    if len(content) > MAX_SIZE:
        raise HTTPException(
            status_code=413,
            detail="Le fichier vidéo dépasse la limite maximale autorisée de 100 Mo.",
        )
    try:
        return job_seeker_service.save_uploaded_video(
            connection, str(current_job_seeker["id"]), file.filename or "pitch.mp4", content
        )
    except ValueError as error:
        raise HTTPException(status_code=422, detail=str(error)) from error


@router.post("/avatar/upload")
async def upload_avatar(
    file: UploadFile = File(...),
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    MAX_SIZE = 5 * 1024 * 1024  # 5 Mo max
    if file.content_type not in {"image/jpeg", "image/png", "image/webp"}:
        raise HTTPException(
            status_code=422,
            detail="Format d'image non supporté (jpeg, png ou webp requis).",
        )
    content = await file.read()
    if len(content) > MAX_SIZE:
        raise HTTPException(
            status_code=413,
            detail="L'image dépasse la limite maximale autorisée de 5 Mo.",
        )
    return job_seeker_service.save_uploaded_avatar(
        connection, str(current_job_seeker["id"]), file.filename or "avatar.jpg", content
    )


@router.get("/interactions")
def get_interactions(
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    return job_seeker_service.get_interactions(
        connection, str(current_job_seeker["id"])
    )


@router.patch("/solicitations/{solicitation_id}/read")
def mark_solicitation_read(
    solicitation_id: str,
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    result = job_seeker_service.set_solicitation_read(
        connection, str(current_job_seeker["id"]), solicitation_id, True
    )
    if result is None:
        raise HTTPException(status_code=404, detail="Solicitation not found")
    return result


@router.patch("/solicitations/{solicitation_id}/unread")
def mark_solicitation_unread(
    solicitation_id: str,
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    result = job_seeker_service.set_solicitation_read(
        connection, str(current_job_seeker["id"]), solicitation_id, False
    )
    if result is None:
        raise HTTPException(status_code=404, detail="Solicitation not found")
    return result


@router.patch("/solicitations/{solicitation_id}/archive")
def archive_solicitation(
    solicitation_id: str,
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    result = job_seeker_service.set_solicitation_archived(
        connection, str(current_job_seeker["id"]), solicitation_id, True
    )
    if result is None:
        raise HTTPException(status_code=404, detail="Solicitation not found")
    return result


@router.patch("/solicitations/{solicitation_id}/unarchive")
def unarchive_solicitation(
    solicitation_id: str,
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    result = job_seeker_service.set_solicitation_archived(
        connection, str(current_job_seeker["id"]), solicitation_id, False
    )
    if result is None:
        raise HTTPException(status_code=404, detail="Solicitation not found")
    return result


@router.post("/solicitations/{solicitation_id}/reply")
def reply_to_solicitation(
    solicitation_id: str,
    payload: SolicitationReplyRequest,
    current_job_seeker: dict = Depends(require_job_seeker),
    connection: Connection = Depends(get_db),
):
    result = job_seeker_service.reply_to_solicitation(
        connection, str(current_job_seeker["id"]), solicitation_id, payload.message
    )
    if result is None:
        raise HTTPException(status_code=404, detail="Solicitation not found")
    return result
