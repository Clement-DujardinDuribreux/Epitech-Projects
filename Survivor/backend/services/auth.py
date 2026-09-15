from uuid import uuid4

from pymysql.connections import Connection

from core.age import MinimumAgeError, calculate_age, validate_minimum_age
from core.security import hash_password, verify_password
from repositories import auth
from schemas.auth import AgeRegisterRequest, LoginRequest, RegisterRequest


class EmailAlreadyUsedError(ValueError):
    pass


def register(connection: Connection, payload: RegisterRequest) -> dict:
    if auth.find_user_by_email(connection, payload.email) is not None:
        raise EmailAlreadyUsedError
    if payload.birthdate is not None:
        validate_minimum_age(payload.birthdate)
    user = auth.create_user(
        connection,
        str(uuid4()),
        payload.model_dump(),
        hash_password(payload.password),
    )
    return user


def register_age(connection: Connection, user_id: str, payload: AgeRegisterRequest) -> dict | None:
    validate_minimum_age(payload.birthdate)
    if not auth.update_birthdate(connection, user_id, payload.birthdate):
        return None
    return auth.find_user_by_id(connection, user_id)


def authenticate(connection: Connection, payload: LoginRequest) -> dict | None:
    user = auth.find_user_by_email(connection, str(payload.email))
    if user is None or not verify_password(payload.password, user["password"]):
        return None
    return user


def get_user(connection: Connection, user_id: str) -> dict | None:
    return auth.find_user_by_id(connection, user_id)


def remove_user(connection: Connection, user_id: str) -> bool:
    return auth.delete_user(connection, user_id)
