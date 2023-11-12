import mysql from 'mysql2';

const db = mysql.createPool({
  host: 'localhost',
  user: 'bankuser',
  password: 'bankpass',
  database: 'bankdb',
});

export default db;
