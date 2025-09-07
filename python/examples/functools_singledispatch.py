from functools import singledispatch

@singledispatch
def size(x) -> int:
    return len(str(x))

@size.register
def _(x: list) -> int:
    return len(x)

if __name__ == "__main__":
    print(size("abc"), size([1,2,3]))