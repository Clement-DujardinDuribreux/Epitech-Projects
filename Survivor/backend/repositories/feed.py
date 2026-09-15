from pymysql.connections import Connection


def _fetch_relations_for_users(
	connection: Connection, user_ids: list[str]
) -> dict[str, dict[str, list[str]]]:
	relations: dict[str, dict[str, list[str]]] = {
		uid: {"competences": [], "sectors": [], "medias": []} for uid in user_ids
	}
	if not user_ids:
		return relations

	placeholders = ", ".join(["%s"] * len(user_ids))
	with connection.cursor() as cursor:
		query_comp = (
			"SELECT user_id, competence FROM competences WHERE user_id IN ("
			+ placeholders
			+ ")"
		)
		cursor.execute(query_comp, tuple(user_ids))
		for row in cursor.fetchall():
			uid = row.get("user_id")
			comp = row.get("competence")
			if uid in relations and comp and comp not in relations[uid]["competences"]:
				relations[uid]["competences"].append(comp)

		query_sec = (
			"SELECT user_id, sector FROM sector WHERE user_id IN ("
			+ placeholders
			+ ")"
		)
		cursor.execute(query_sec, tuple(user_ids))
		for row in cursor.fetchall():
			uid = row.get("user_id")
			sec = row.get("sector")
			if uid in relations and sec and sec not in relations[uid]["sectors"]:
				relations[uid]["sectors"].append(sec)

		query_med = (
			"SELECT user_id, link FROM media WHERE valid = 1 AND user_id IN ("
			+ placeholders
			+ ")"
		)
		cursor.execute(query_med, tuple(user_ids))
		for row in cursor.fetchall():
			uid = row.get("user_id")
			lnk = row.get("link")
			if uid in relations and lnk and lnk not in relations[uid]["medias"]:
				relations[uid]["medias"].append(lnk)

	return relations


def _build_filter_conditions(
	competences: list[str] = [],
	localisation: str | None = None,
	sector: str | None = None,
	certified: bool | None = None,
) -> tuple[list[str], list]:
	conditions = ["u.role = 'job_seeker'"]
	params: list = []

	for comp in competences:
		clean_comp = comp.strip() if isinstance(comp, str) else ""
		if clean_comp:
			conditions.append(
				"EXISTS (SELECT 1 FROM competences c WHERE c.user_id = u.id AND c.competence = %s)"
			)
			params.append(clean_comp)

	if localisation is not None and localisation.strip():
		conditions.append("u.localisation = %s")
		params.append(localisation.strip())

	if sector is not None and sector.strip():
		conditions.append(
			"EXISTS (SELECT 1 FROM sector s WHERE s.user_id = u.id AND s.sector = %s)"
		)
		params.append(sector.strip())

	if certified is not None:
		conditions.append("u.certificated = %s")
		params.append(int(certified))

	return conditions, params


def get_feed(
	connection: Connection,
	competences: list[str] = [],
	localisation: str | None = None,
	sector: str | None = None,
	certified: bool | None = None,
	page: int = 1,
	limit: int = 20,
) -> list[dict]:
	if page < 1:
		page = 1
	if limit < 1:
		limit = 20
	offset = (page - 1) * limit

	conditions, params = _build_filter_conditions(
		competences=competences,
		localisation=localisation,
		sector=sector,
		certified=certified,
	)

	where_clause = " AND ".join(conditions)
	query = (
		"SELECT u.id, u.email, u.firstname, u.name, u.role, u.localisation, u.certificated, "
		"u.birthdate, u.headline, u.bio, u.avatar_url, u.created_at, u.last_update_at "
		"FROM users u "
		"WHERE " + where_clause + " "
		"ORDER BY u.last_update_at DESC, u.id ASC "
		"LIMIT %s OFFSET %s"
	)
	params.extend([limit, offset])

	with connection.cursor() as cursor:
		cursor.execute(query, tuple(params))
		users = cursor.fetchall()

	if not users:
		return []

	user_ids = [u["id"] for u in users]
	relations = _fetch_relations_for_users(connection, user_ids)

	for user in users:
		rel = relations.get(user["id"], {"competences": [], "sectors": [], "medias": []})
		user["competences"] = rel["competences"]
		user["sectors"] = rel["sectors"]
		user["medias"] = rel["medias"]

	return users


def count_feed(
	connection: Connection,
	competences: list[str] = [],
	localisation: str | None = None,
	sector: str | None = None,
	certified: bool | None = None,
) -> int:
	conditions, params = _build_filter_conditions(
		competences=competences,
		localisation=localisation,
		sector=sector,
		certified=certified,
	)

	where_clause = " AND ".join(conditions)
	query = "SELECT COUNT(*) AS total FROM users u WHERE " + where_clause

	with connection.cursor() as cursor:
		cursor.execute(query, tuple(params))
		row = cursor.fetchone()
		if not row:
			return 0
		if isinstance(row, dict):
			return row.get("total", 0)
		return row[0]


def get_user_feed(connection: Connection, user_id: str) -> dict | None:
	query = (
		"SELECT u.id, u.email, u.firstname, u.name, u.role, u.localisation, u.certificated, "
		"u.birthdate, u.headline, u.bio, u.avatar_url, u.created_at, u.last_update_at "
		"FROM users u "
		"WHERE u.id = %s AND u.role = 'job_seeker'"
	)
	with connection.cursor() as cursor:
		cursor.execute(query, (user_id,))
		user = cursor.fetchone()
		if not user:
			return None

	relations = _fetch_relations_for_users(connection, [user_id])
	rel = relations.get(user_id, {"competences": [], "sectors": [], "medias": []})
	user["competences"] = rel["competences"]
	user["sectors"] = rel["sectors"]
	user["medias"] = rel["medias"]
	return user


def record_profile_view(
	connection: Connection, job_seeker_id: str, recruiter_id: str | None = None
) -> bool:
	from uuid import uuid4
	query = """
		INSERT INTO profile_views (id, job_seeker_id, recruiter_id)
		VALUES (%s, %s, %s)
	"""
	with connection.cursor() as cursor:
		cursor.execute(query, (str(uuid4()), job_seeker_id, recruiter_id))
		return cursor.rowcount > 0
