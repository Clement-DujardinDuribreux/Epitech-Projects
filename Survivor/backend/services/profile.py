from pymysql.connections import Connection
from pymysql.err import IntegrityError

from repositories import profile as profile_repo


class FollowTargetNotFoundError(ValueError):
    pass


class FollowNotFoundError(ValueError):
    pass


def follow_user(connection: Connection, recruiter_id: str, job_seeker_id: str) -> dict:
    if profile_repo.find_job_seeker_by_id(connection, job_seeker_id) is None:
        raise FollowTargetNotFoundError

    follow = profile_repo.find_follow(connection, recruiter_id, job_seeker_id)
    if follow is not None:
        return follow

    try:
        return profile_repo.create_follow(connection, recruiter_id, job_seeker_id)
    except IntegrityError:
        follow = profile_repo.find_follow(connection, recruiter_id, job_seeker_id)
        if follow is not None:
            return follow
        raise


def unfollow_user(connection: Connection, recruiter_id: str, job_seeker_id: str) -> bool:
    if not profile_repo.delete_follow(connection, recruiter_id, job_seeker_id):
        raise FollowNotFoundError
    return True


def get_followed_users(connection: Connection, recruiter_id: str) -> dict:
    users = profile_repo.list_followed_users(connection, recruiter_id)
    total = profile_repo.count_followed_users(connection, recruiter_id)
    return {
        "data": users,
        "total": total,
    }
