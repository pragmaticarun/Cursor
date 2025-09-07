def describe(value):
    match value:
        case {"type": "point", "x": x, "y": y}:
            return f"Point({x},{y})"
        case [x, y]:
            return f"Pair({x},{y})"
        case str() as s if s.isdigit():
            return f"Digits:{s}"
        case _:
            return "Unknown"

if __name__ == "__main__":
    print(describe({"type":"point","x":1,"y":2}))
    print(describe([3,4]))
    print(describe("123"))