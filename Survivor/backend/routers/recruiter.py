from fastapi import APIRouter, Depends, HTTPException
from pymysql.connections import Connection

from dependencies.auth import require_recruiter
from dependencies.database import get_db
from schemas.recruiter import CreateSolicitationRequest, UpdateSolicitationStatusRequest
from services import recruiter as recruiter_service

router = APIRouter(prefix="/recruiter", tags=["recruiter"])


@router.post("/solicitations", status_code=201)
def create_solicitation(
    payload: CreateSolicitationRequest,
    current_recruiter: dict = Depends(require_recruiter),
    connection: Connection = Depends(get_db),
):
    return recruiter_service.send_solicitation(
        connection, str(current_recruiter["id"]), payload
    )


@router.get("/solicitations")
def list_solicitations(
    current_recruiter: dict = Depends(require_recruiter),
    connection: Connection = Depends(get_db),
):
    return recruiter_service.get_solicitations(
        connection, str(current_recruiter["id"])
    )


@router.patch("/solicitations/{solicitation_id}/status")
def update_solicitation_status(
    solicitation_id: str,
    payload: UpdateSolicitationStatusRequest,
    current_recruiter: dict = Depends(require_recruiter),
    connection: Connection = Depends(get_db),
):
    updated = recruiter_service.update_status(
        connection, str(current_recruiter["id"]), solicitation_id, payload
    )
    if not updated:
        raise HTTPException(status_code=404, detail="Solicitation not found")
    return updated


@router.delete("/solicitations/{solicitation_id}", status_code=204)
def delete_solicitation(
    solicitation_id: str,
    current_recruiter: dict = Depends(require_recruiter),
    connection: Connection = Depends(get_db),
):
    deleted = recruiter_service.delete_solicitation(
        connection, str(current_recruiter["id"]), solicitation_id
    )
    if not deleted:
        raise HTTPException(status_code=404, detail="Solicitation not found")
