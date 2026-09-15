import os

import pymysql
from pymysql.connections import Connection
from pymysql.cursors import DictCursor

def create_connection() -> Connection:
    return pymysql.connect(
        host=os.getenv("MYSQL_HOST"),
        port=int(os.getenv("MYSQL_PORT")),
        user=os.getenv("MYSQL_USER"),
        password=os.getenv("MYSQL_PASSWORD"),
        database=os.getenv("MYSQL_DATABASE"),
        charset="utf8mb4",
        cursorclass=DictCursor,
        autocommit=True,
    )
