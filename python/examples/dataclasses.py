from dataclasses import dataclass

@dataclass(slots=True, frozen=True)
class Vector:
    x: float
    y: float

    def __add__(self, other: "Vector") -> "Vector":
        return Vector(self.x + other.x, self.y + other.y)

if __name__ == "__main__":
    a = Vector(1,2)
    b = Vector(3,4)
    print(a + b)