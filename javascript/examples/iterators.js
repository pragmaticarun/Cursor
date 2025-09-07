// Custom iterable and async iterable
export const range = {
  [Symbol.iterator]() {
    let i = 0
    return {
      next() {
        return i < 3 ? { value: i++, done: false } : { done: true }
      }
    }
  },
}

export async function* asyncRange(n) {
  for (let i = 0; i < n; i++) {
    await new Promise(r => setTimeout(r, 1))
    yield i
  }
}

if (import.meta.url === `file://${process.argv[1]}`) {
  console.log([...range])
  for await (const x of asyncRange(3)) console.log(x)
}