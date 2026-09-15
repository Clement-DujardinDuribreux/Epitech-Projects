from datetime import datetime
from typing import Literal
from pydantic import BaseModel, Field

SolicitationStatus = Literal["sent", "contacted", "interview", "rejected"]


class CreateSolicitationRequest(BaseModel):
    candidate_id: str = Field(min_length=1, max_length=64)
    message: str = Field(min_length=1, max_length=2000)


class UpdateSolicitationStatusRequest(BaseModel):
    status: SolicitationStatus
