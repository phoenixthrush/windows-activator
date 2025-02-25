import re
import base64
import os
import argparse


def encode_file_to_base64(filepath):
    if not os.path.isfile(filepath):
        return None
    with open(filepath, "rb") as f:
        return base64.b64encode(f.read()).decode("utf-8")


def replace_file_with_base64(match):
    filepath = os.path.join("site", match.group(1))
    file_ext = os.path.splitext(filepath)[1][1:]
    base64_data = encode_file_to_base64(filepath)
    if base64_data:
        mime_type = (
            f"image/{file_ext}" if file_ext in ["png", "jpg", "jpeg", "gif", "webp"]
            else "audio/mpeg" if file_ext == "mp3"
            else "application/octet-stream"
        )
        return f'src="data:{mime_type};base64,{base64_data}"'
    print(f"Warning: File '{filepath}' not found. Skipping base64 encoding.")
    return match.group(0)


def modify_content(content):
    return re.sub(r'src="([^"]+)"', replace_file_with_base64, content)


def process_html_file(input_filename, output_filename):
    with open(input_filename, "r", encoding="utf-8") as f:
        original_content = f.read()
    modified_content = modify_content(original_content)
    with open(output_filename, "w", encoding="utf-8") as f:
        f.write(modified_content)
    print(f"Saved as {output_filename}")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Convert file references in HTML to base64 data URIs.")
    parser.add_argument("-i", "--input", required=True, help="The input HTML file to process")
    parser.add_argument("-o", "--output", required=True, help="The output file where modified HTML will be saved")

    args = parser.parse_args()

    process_html_file(args.input, args.output)
