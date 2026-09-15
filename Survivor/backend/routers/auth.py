from fastapi import APIRouter, Depends, HTTPException, Request

from pymysql.connections import Connection

from core.security import create_access_token, get_token_subject, get_token_request
from dependencies.database import get_db
from schemas.auth import LoginRequest, RegisterRequest, AgeRegisterRequest
from services.auth import EmailAlreadyUsedError, MinimumAgeError, authenticate, calculate_age, get_user, register, register_age as register_age_for_user, remove_user


router = APIRouter(prefix="/auth", tags=["auth"])

def to_public_user(user: dict) -> dict:
    return {key: value for key, value in user.items() if key != "password"}


@router.post("/register", status_code=201)
def register_user(payload: RegisterRequest, connection: Connection = Depends(get_db)):
    try:
        user = register(connection, payload)
    except MinimumAgeError:
        raise HTTPException(status_code=422, detail="You must be at least 16 years old")
    except EmailAlreadyUsedError:
        raise HTTPException(status_code=409, detail="Email already registered")
    return {"access_token": create_access_token(user["id"]), "user": to_public_user(user)}

@router.post("/login")
def login(payload: LoginRequest, connection: Connection = Depends(get_db)):
    user = authenticate(connection, payload)
    if user is None:
        raise HTTPException(status_code=401, detail="Invalid email or password")
    return {"access_token": create_access_token(user["id"]), "user": to_public_user(user)}

@router.get("/me")
def get_me(request: Request, connection: Connection = Depends(get_db)):
    id = get_token_subject(get_token_request(request))
    if id is None:
        raise HTTPException(status_code=401, detail="No User found")
    user = get_user(connection, id)
    if user is None:
        raise HTTPException(status_code=401, detail="No User found")
    return to_public_user(user)

@router.get("/me/age-status")
def get_age_status(request: Request, connection: Connection = Depends(get_db)):
    user_id = get_token_subject(get_token_request(request))
    if user_id is None:
        raise HTTPException(status_code=401, detail="Invalid token")
    user = get_user(connection, user_id)
    if user is None:
        raise HTTPException(status_code=401, detail="Invalid token")
    birthdate = user["birthdate"]
    return {"valid": calculate_age(birthdate) >= 16 if birthdate else None}

@router.patch("/me/birthdate")
def register_age(request: Request, payload: AgeRegisterRequest, connection: Connection = Depends(get_db)):
    user_id = get_token_subject(get_token_request(request))
    if user_id is None:
        raise HTTPException(status_code=401, detail="Invalid token")
    try:
        user = register_age_for_user(connection, user_id, payload)
    except MinimumAgeError:
        raise HTTPException(status_code=422, detail="You must be at least 16 years old")
    if user is None:
        raise HTTPException(status_code=404, detail="No user found")
    return to_public_user(user)

@router.delete("/delete", status_code=204)
def delete_account(request: Request, connection: Connection = Depends(get_db)):
    id = get_token_subject(get_token_request(request))
    if id is None:
        raise HTTPException(status_code=401, detail="Invalid token")
    if not remove_user(connection, id):
        raise HTTPException(status_code=404, detail="No user found")
