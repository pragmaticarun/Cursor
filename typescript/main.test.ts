import { describe, expect, it } from 'vitest'

// Re-import compiled file would require build; test simple types here

describe('typescript basics', () => {
  it('string template works', () => {
    const name = 'Ada'
    expect(`Hello, ${name}!`).toBe('Hello, Ada!')
  })
})