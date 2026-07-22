"""Convert a binary file into a null-terminated C byte array."""

import argparse
import re
from pathlib import Path


BYTES_PER_LINE = 12


def c_identifier(path: Path) -> str:
    """Turn a filename into a valid, predictable C identifier."""
    name = re.sub(r"\W", "_", path.stem)
    if name[:1].isdigit():
        name = "_" + name
    return name


def file_to_c_array(input_path: Path, output_path: Path):
    data = input_path.read_bytes()
    name = c_identifier(input_path)

    rows = []
    for offset in range(0, len(data), BYTES_PER_LINE):
        chunk = data[offset : offset + BYTES_PER_LINE]
        rows.append(", ".join(f"0x{byte:02x}" for byte in chunk))

    body = ",\n  ".join(rows)
    if body:
        body += ",\n  "

    source = (
        f"unsigned char {name}[] = {{\n  {body}0x00\n}};\n"
        f"unsigned int {name}_len = {len(data) + 1};\n"
    )
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(source, encoding="utf-8")


def parse_args():
    parser = argparse.ArgumentParser(
        description="Convert a file into a C byte array."
    )
    parser.add_argument("-i", "--input", type=Path, required=True)
    parser.add_argument("-o", "--output", type=Path, required=True)
    return parser.parse_args()


def main():
    args = parse_args()
    file_to_c_array(args.input, args.output)
    print(f"Output written to: {args.output.resolve()}")


if __name__ == "__main__":
    main()
