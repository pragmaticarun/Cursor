#!/usr/bin/env node
import { readFile, writeFile } from 'node:fs/promises';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { setTimeout as sleep } from 'node:timers/promises';

console.log('=== JavaScript demo ===');

// Destructuring and spread
const user = { id: 1, name: 'Ada', email: 'ada@example.com' };
const { email, ...rest } = user;
console.log('User without email:', rest);

// Array methods
const nums = [1,2,3,4,5];
console.log('Sum:', nums.reduce((a, b) => a + b, 0));

// Map/Set
const seen = new Set(nums);
const index = new Map(nums.map((n, i) => [n, i]));
console.log('Seen has 3:', seen.has(3), 'Index of 4:', index.get(4));

// RegExp
const text = 'hello HELLO world';
console.log('Matches:', text.match(/hello/gi));

// Async/await and file IO
const p = join(tmpdir(), 'demo.json');
await writeFile(p, JSON.stringify({ user }, null, 2));
console.log('Wrote', p);
console.log('Read back length:', (await readFile(p, 'utf8')).length);

// Top-level await with a sleep
await sleep(10);
console.log('Done.');