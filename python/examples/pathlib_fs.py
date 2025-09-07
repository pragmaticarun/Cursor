from pathlib import Path

def list_py(root: Path) -> list[Path]:
    return [p for p in root.rglob("*.py")]

if __name__ == "__main__":
    here = Path(__file__).resolve().parent
    for p in list_py(here.parent)[:3]:
        print(p)