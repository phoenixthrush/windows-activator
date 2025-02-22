import re
import base64
import os
import sys


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
        mime_type = f"image/{file_ext}" if file_ext in ["png", "jpg", "jpeg", "gif", "webp"] else "application/octet-stream"
        return f'src="data:{mime_type};base64,{base64_data}"'
    print(f"Warning: File '{filepath}' not found. Skipping base64 encoding.")
    return match.group(0)


def modify_content(content):
    return re.sub(r'src="([^"]+)"', replace_file_with_base64, content)


def process_html_file(input_filename):
    with open(input_filename, "r", encoding="utf-8") as f:
        original_content = f.read()
    modified_content = modify_content(original_content)
    modified_html_filename = f"./{os.path.splitext(os.path.basename(input_filename))[0]}.modified.html"
    with open(modified_html_filename, "w", encoding="utf-8") as f:
        f.write(modified_content)
    print(f"Saved as {modified_html_filename}")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: {} <input_filename>".format(sys.argv[0]))
        sys.exit(1)
    process_html_file(sys.argv[1])
