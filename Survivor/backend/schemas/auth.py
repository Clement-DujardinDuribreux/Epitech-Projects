from datetime import date
from typing import Literal

from pydantic import BaseModel, EmailStr, Field


class RegisterRequest(BaseModel):
    email: EmailStr
    password: str = Field(min_length=8)
    firstname: str
    name: str
    role: Literal["job_seeker", "recruiter"] = "job_seeker"
    localisation: str | None = None
    birthdate: date | None = None

class AgeRegisterRequest(BaseModel):
    birthdate: date

class LoginRequest(BaseModel):
    email: EmailStr
    password: str
