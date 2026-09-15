from datetime import date
from typing import Literal
from pydantic import BaseModel, Field

Role = Literal["job_seeker", "recruiter", "admin"]


class JobSeekerProfileUpdate(BaseModel):
    firstname: str
    name: str
    localisation: str | None = None
    birthdate: date | None = None
    headline: str | None = None
    bio: str | None = None
    phone: str | None = None
    avatar_url: str | None = None


class SolicitationReplyRequest(BaseModel):
    message: str = Field(min_length=1, max_length=2000)


class JobSeekerCompetencesUpdate(BaseModel):
    add: list[str] = Field(default_factory=list)
    remove: list[str] = Field(default_factory=list)


class JobSeekerSectorsUpdate(BaseModel):
    add: list[str] = Field(default_factory=list)
    remove: list[str] = Field(default_factory=list)


class JobSeekerVideoUpdate(BaseModel):
    link: str = Field(min_length=5, max_length=500)


class CertificationValidationRequest(BaseModel):
    score: int = Field(ge=0, le=20)
    total: int = Field(default=20, ge=1)
