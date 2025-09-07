-- SQLite demo
DROP TABLE IF EXISTS users;
CREATE TABLE users (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  email TEXT UNIQUE
);

INSERT INTO users (name, email) VALUES
  ('Ada', 'ada@example.com'),
  ('Alan', 'alan@example.com');

SELECT id, name FROM users ORDER BY id;

-- Aggregation
SELECT COUNT(*) AS user_count FROM users;