type Id = number & { readonly brand: unique symbol };
const makeId = (n: number) => n as Id;

interface User {
  id: Id;
  name: string;
  email?: string;
}

function greet(user: User): string {
  return `Hello, ${user.name}!`;
}

function mapValues<T, U>(obj: Record<string, T>, fn: (v: T) => U): Record<string, U> {
  const out: Record<string, U> = {};
  for (const [k, v] of Object.entries(obj)) out[k] = fn(v as T);
  return out;
}

async function main(): Promise<void> {
  const ada: User = { id: makeId(1), name: 'Ada' };
  console.log(greet(ada));

  const scores = { alice: 10, bob: 7 };
  console.log(mapValues(scores, n => n * 2));

  // Narrowing
  function len(x: string | string[]): number { return typeof x === 'string' ? x.length : x.length; }
  console.log('len("hi") =', len('hi'));

  // Top-level await compatible when compiled as module
}

main().catch(err => { console.error(err); process.exit(1); });