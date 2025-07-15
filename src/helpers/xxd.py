import argparse
from pathlib import Path

def file_to_c_array(input_path: Path, output_path: Path):
    data = input_path.read_bytes()
    name = input_path.stem.replace(".", "_")
    lines = (
        ", ".join(f"0x{b:02x}" for b in data[i:i+12])
        for i in range(0, len(data), 12)
    )
    out = (
        f"unsigned char {name}[] = {{\n  "
        + ",\n  ".join(lines)
        + ", 0x00\n};\n"
        + f"unsigned int {name}_len = {len(data)+1};\n"
    )
    output_file = output_path or input_path.with_suffix(".c")
    output_file.write_text(out)
    print(f"Output written to: {output_file.resolve()}")

def main():
    p = argparse.ArgumentParser()
    p.add_argument("-i", "--input", type=Path, required=True)
    p.add_argument("-o", "--output", type=Path, required=True)
    args = p.parse_args()

    file_to_c_array(args.input, args.output)

if __name__ == "__main__":
    main()