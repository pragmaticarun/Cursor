import { describe, expect, it } from 'vitest'
import { wordCount } from './utils.js'

describe('wordCount', () => {
  it('counts case-insensitively', () => {
    const c = wordCount('Hello hello world')
    expect(c.hello).toBe(2)
    expect(c.world).toBe(1)
  })
})