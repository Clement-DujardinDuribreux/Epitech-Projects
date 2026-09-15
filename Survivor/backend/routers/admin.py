from fastapi import APIRouter, Depends, HTTPException
from pymysql.connections import Connection

from dependencies.auth import require_admin
from dependencies.database import get_db
from schemas.admin import MediaRejectRequest
from services import admin as admin_service

router = APIRouter(prefix="/admin", tags=["admin"])


@router.get("/kpis")
def get_kpis(
    current_admin: dict = Depends(require_admin),
    connection: Connection = Depends(get_db),
):
    return admin_service.get_kpis(connection)


@router.get("/media/pending")
def list_pending_media(
    current_admin: dict = Depends(require_admin),
    connection: Connection = Depends(get_db),
):
    return admin_service.get_pending_media(connection)


@router.patch("/media/valid/{media_id}")
def validate_media_route(
    media_id: str,
    current_admin: dict = Depends(require_admin),
    connection: Connection = Depends(get_db),
):
    media = admin_service.validate_media(connection, media_id, str(current_admin["id"]))
    if media is None:
        raise HTTPException(status_code=404, detail="Media not found")
    return media


@router.patch("/media/reject/{media_id}")
def reject_media_route(
    media_id: str,
    payload: MediaRejectRequest,
    current_admin: dict = Depends(require_admin),
    connection: Connection = Depends(get_db),
):
    reason = payload.reason
    if payload.comment:
        reason = f"{payload.reason} — {payload.comment}"
    success = admin_service.reject_media(connection, media_id, str(current_admin["id"]), reason)
    if not success:
        raise HTTPException(status_code=404, detail="Media not found")
    return {"status": "rejected", "id": media_id, "reason": reason}
