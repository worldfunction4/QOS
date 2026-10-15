-- login_info 定义

CREATE TABLE IF NOT EXISTS "login_info" (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT(100) NOT NULL UNIQUE,          -- 设置唯一键
    secret_id TEXT(100) NOT NULL UNIQUE,     -- 设置唯一键
    secret_key TEXT(100) NOT NULL,
    remark TEXT(200),
    timestamp INTEGER NOT NULL
);