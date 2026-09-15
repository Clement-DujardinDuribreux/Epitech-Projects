from pymysql.connections import Connection


def create_user(connection: Connection, user_id: str, user: dict, password_hash: str) -> dict:
    with connection.cursor() as cursor:
        cursor.execute(
            "INSERT INTO users (id, email, firstname, name, password, role, localisation, birthdate) VALUES (%s, %s, %s, %s, %s, %s, %s, %s)",
            (user_id, user["email"], user["firstname"], user["name"], password_hash, user["role"], user["localisation"], user["birthdate"],),
        )
    return find_user_by_id(connection, user_id)


def find_user_by_email(connection: Connection, email: str) -> dict | None:
    with connection.cursor() as cursor:
        cursor.execute("SELECT * FROM users WHERE email = %s", (email,))
        return cursor.fetchone()


def find_user_by_id(connection: Connection, user_id: str) -> dict | None:
    with connection.cursor() as cursor:
        cursor.execute("SELECT * FROM users WHERE id = %s", (user_id,))
        return cursor.fetchone()


def delete_user(connection: Connection, user_id: str) -> bool:
    with connection.cursor() as cursor:
        cursor.execute("DELETE FROM users WHERE id = %s", (user_id,))
        return cursor.rowcount == 1


def update_birthdate(connection: Connection, user_id: str, birthdate: object) -> bool:
    with connection.cursor() as cursor:
        cursor.execute("UPDATE users SET birthdate = %s WHERE id = %s", (birthdate, user_id))
        return cursor.rowcount == 1
