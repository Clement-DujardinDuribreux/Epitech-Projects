from core.database import create_connection



def get_db():
    connection = create_connection()
    try:
        yield connection
    finally:
        connection.close()
