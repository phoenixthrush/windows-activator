import os
import argparse


def convert_file_to_c_array(input_file, output_file=None):
    if output_file is None:
        output_file = os.path.splitext(os.path.basename(input_file))[0] + ".c"
    with open(input_file, "rb") as file_handle:
        file_bytes = file_handle.read()
    c_array_name = os.path.basename(input_file).replace(".", "_")
    hex_byte_strings = [
        "0x{:02x}".format(byte) for byte in file_bytes
    ] + ["0x00"]
    lines = [
        "  " + ", ".join(hex_byte_strings[i:i+12])
        for i in range(0, len(hex_byte_strings), 12)
    ]
    output_content = "\n".join(
        [
            f"unsigned char {c_array_name}[] = {{",
            ",\n".join(
                lines
            ), "};", f"unsigned int {c_array_name}_len = {len(file_bytes) + 1};"
        ]
    )
    with open(output_file, "w") as out_handle:
        out_handle.write(output_content + "\n")
    print(f"Saved as ./{output_file}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert file to C array.")
    parser.add_argument("-i", "--input", required=True,
                        help="The input file to convert")
    parser.add_argument(
        "-o", "--output", help="The output C file (default is input file name with .c extension)")
    args = parser.parse_args()
    convert_file_to_c_array(args.input, args.output)
