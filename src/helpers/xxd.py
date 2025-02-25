import os
import argparse


def convert(filename, output_filename=None):
    if output_filename is None:
        output_filename = os.path.splitext(os.path.basename(filename))[0] + ".c"
    with open(filename, "rb") as f:
        data = f.read()
    c_array_name = os.path.basename(filename).replace(".", "_")
    hex_values = ["0x{:02x}".format(byte) for byte in data]
    hex_values.append("0x00")
    with open(output_filename, "w") as f:
        f.write("unsigned char {}[] = {{\n".format(c_array_name))
        for i in range(0, len(hex_values), 12):
            group = hex_values[i:i+12]
            f.write("  " + ", ".join(group))
            if i + 12 < len(hex_values):
                f.write(",\n")
            else:
                f.write("\n")
        f.write("};\n")
        f.write("unsigned int {}_len = {};\n".format(c_array_name, len(data) + 1))
        print(f"Saved as ./{output_filename}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert file to C array.")
    parser.add_argument("-i", "--input", required=True, help="The input file to convert")
    parser.add_argument("-o", "--output", help="The output C file (default is input file name with .c extension)")

    args = parser.parse_args()

    convert(args.input, args.output)
