type Ok = { kind: 'ok'; value: string }
type Err = { kind: 'err'; error: string }
type Result = Ok | Err

export function handle(r: Result): string {
  return r.kind === 'ok' ? r.value : `error:${r.error}`
}

const routes = {
  home: '/home',
  user: (id: number) => `/user/${id}`,
} satisfies Record<string, string | ((...a: any[]) => string)>

export type EventName<T extends string> = `${T}:changed`
const e: EventName<'user'> = 'user:changed'
console.log(e, handle({ kind: 'ok', value: 'done' }))