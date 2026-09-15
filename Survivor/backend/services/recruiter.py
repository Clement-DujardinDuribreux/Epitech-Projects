from pymysql.connections import Connection

from repositories import recruiter as recruiter_repo
from schemas.recruiter import CreateSolicitationRequest, UpdateSolicitationStatusRequest


def send_solicitation(
    connection: Connection, recruiter_id: str, payload: CreateSolicitationRequest
) -> dict:
    return recruiter_repo.create_solicitation(
        connection, recruiter_id, payload.candidate_id, payload.message
    )


def get_solicitations(connection: Connection, recruiter_id: str) -> list[dict]:
    return recruiter_repo.list_solicitations_by_recruiter(connection, recruiter_id)


def update_status(
    connection: Connection,
    recruiter_id: str,
    solicitation_id: str,
    payload: UpdateSolicitationStatusRequest,
) -> dict | None:
    updated = recruiter_repo.update_solicitation_status(
        connection, solicitation_id, recruiter_id, payload.status
    )
    if not updated:
        return None
    return recruiter_repo.find_solicitation_by_id(connection, solicitation_id)


def delete_solicitation(
    connection: Connection, recruiter_id: str, solicitation_id: str
) -> bool:
    return recruiter_repo.delete_solicitation(connection, solicitation_id, recruiter_id)
