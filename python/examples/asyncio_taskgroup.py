import asyncio

async def work(i: int) -> int:
    await asyncio.sleep(0.01)
    return i * i

async def main() -> None:
    results = []
    async with asyncio.TaskGroup() as tg:
        tasks = [tg.create_task(work(i)) for i in range(5)]
    for t in tasks:
        results.append(t.result())
    print(results)

if __name__ == "__main__":
    asyncio.run(main())