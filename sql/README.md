# SQL Tutorial: Essentials with SQLite

## Quick start

```bash
sqlite3 demo.db < sql/example.sql
```

## Language basics

- DDL: `CREATE`, `ALTER`, `DROP`
- DML: `INSERT`, `UPDATE`, `DELETE`
- Queries: `SELECT`, `WHERE`, `GROUP BY`, `HAVING`, `ORDER BY`, `LIMIT`
- Joins: `INNER`, `LEFT`, `CROSS`
- Constraints: `PRIMARY KEY`, `UNIQUE`, `CHECK`, `FOREIGN KEY`
- Indices: `CREATE INDEX`, `EXPLAIN QUERY PLAN`

## SQLite specifics

- Types are dynamic; column affinities
- `WITHOUT ROWID` tables
- Common table expressions (CTEs) using `WITH`
- Window functions (3.25+): `ROW_NUMBER`, `RANK`, `SUM() OVER (...)`

See `sql/example.sql` for runnable examples.

---

## Advanced SQL topics

- Subqueries: scalar, correlated, `EXISTS`
- Set ops: `UNION`, `INTERSECT`, `EXCEPT`
- Analytic/window functions: partitions and frames
- Transactions: `BEGIN`, `COMMIT`, `ROLLBACK`; isolation notes for SQLite
- Query optimization: indices, covering indices, statistics
- Pragmas in SQLite: `PRAGMA foreign_keys=ON`, `journal_mode`, `synchronous`

## References

- SQLite docs: `https://www.sqlite.org/docs.html`
- Use `EXPLAIN QUERY PLAN` to understand performance