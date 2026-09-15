from pydantic import BaseModel, Field


class MediaRejectRequest(BaseModel):
    reason: str = Field(min_length=1, max_length=500)
    comment: str | None = Field(default=None, max_length=1000)

