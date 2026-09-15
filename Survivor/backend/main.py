from pathlib import Path
import pymysql
from fastapi import FastAPI, status
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import JSONResponse
from fastapi.staticfiles import StaticFiles

from core.database import create_connection
from routers.admin import router as admin_router
from routers.auth import router as auth_router
from routers.feed import router as feed_router
from routers.job_seeker import router as job_seeker_router
from routers.profile import router as profile_router
from routers.recruiter import router as recruiter_router

app = FastAPI(title="API ProfilsActifs", version="1.0.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=[
        "http://localhost:5173",
        "http://127.0.0.1:5173",
        "http://localhost:80",
        "http://localhost:10001",
        "http://127.0.0.1:10001",
        "http://localhost",
    ],
    allow_origin_regex=r"^https?://(localhost|127\.0\.0\.1)(:[0-9]+)?$",
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Montage du dossier uploads pour les vidéos en direct
upload_dir = Path("/app/uploads")
if not upload_dir.exists():
    upload_dir = Path("./uploads")
upload_dir.mkdir(parents=True, exist_ok=True)
app.mount("/uploads", StaticFiles(directory=str(upload_dir)), name="uploads")

app.include_router(auth_router)
app.include_router(feed_router)
app.include_router(job_seeker_router)
app.include_router(recruiter_router)
app.include_router(admin_router)
app.include_router(profile_router)

@app.get("/")
async def root():
    return {"message": "API ProfilsActifs"}

@app.get("/health")
def health():
    connection = None
    try:
        connection = create_connection()
        connection.cursor().execute("SELECT 1")
    except (ValueError, pymysql.MySQLError):
        return JSONResponse(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            content={"status": "ko"},
        )
    if connection is not None:
        connection.close()
    return {"status": "ok"}
