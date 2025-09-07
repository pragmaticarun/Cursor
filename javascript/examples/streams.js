import { Readable, Transform } from 'node:stream'
import { pipeline } from 'node:stream/promises'

const src = Readable.from(['Hello', ' ', 'World'])
const upper = new Transform({
  transform(chunk, _enc, cb) { cb(null, chunk.toString().toUpperCase()) }
})

await pipeline(src, upper, process.stdout)