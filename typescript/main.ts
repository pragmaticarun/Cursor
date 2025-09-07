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

// Discriminated union
interface LoadOk { kind: 'ok'; value: string }
interface LoadErr { kind: 'err'; error: Error }
type LoadResult = LoadOk | LoadErr;

function describe(result: LoadResult): string {
  switch (result.kind) {
    case 'ok': return `loaded: ${result.value}`;
    case 'err': return `error: ${result.error.message}`;
  }
}

// Mapped type
type ReadonlyProps<T> = { readonly [K in keyof T]: T[K] };

async function main(): Promise<void> {
  const ada: User = { id: makeId(1), name: 'Ada' };
  console.log(greet(ada));

  const scores = { alice: 10, bob: 7 };
  console.log(mapValues(scores, n => n * 2));

  const results: LoadResult[] = [{ kind: 'ok', value: 'data' }, { kind: 'err', error: new Error('boom') }];
  for (const r of results) console.log(describe(r));

  const ro: ReadonlyProps<User> = { id: makeId(2), name: 'Alan' };
  console.log('readonly name =', ro.name);
}

main().catch(err => { console.error(err); process.exit(1); });