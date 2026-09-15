from datetime import date


class MinimumAgeError(ValueError):
    pass


def calculate_age(birthdate: date) -> int:
    today = date.today()
    return today.year - birthdate.year - (
        (today.month, today.day) < (birthdate.month, birthdate.day)
    )


def validate_minimum_age(birthdate: date) -> None:
    if calculate_age(birthdate) < 16:
        raise MinimumAgeError
