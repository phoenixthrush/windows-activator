import re
import base64
import os
import argparse

# https://docs.rs/minify-html/latest/minify_html/struct.Cfg.html
import minify_html


def convert_file_to_base64(file_path):
    if not os.path.isfile(file_path):
        return None
    with open(file_path, "rb") as binary_file:
        return base64.b64encode(binary_file.read()).decode("utf-8")


def replace_src_with_base64(regex_match):
    file_path = os.path.join("site", regex_match.group(1))
    file_extension = os.path.splitext(file_path)[1][1:]
    base64_string = convert_file_to_base64(file_path)
    if base64_string:
        mime_type = (
            f"image/{file_extension}" if file_extension in ["png", "jpg", "jpeg", "gif", "webp"]
            else "audio/mpeg" if file_extension == "mp3"
            else "application/octet-stream"
        )
        return f'src="data:{mime_type};base64,{base64_string}"'
    print(f"Warning: File '{file_path}' not found. Skipping base64 encoding.")
    return regex_match.group(0)


def embed_css_inline(regex_match):
    css_file_path = os.path.join("site", regex_match.group(1))
    if os.path.isfile(css_file_path):
        with open(css_file_path, "r", encoding="utf-8") as css_file:
            css_text = css_file.read()
        return f'<style>{css_text}</style>'
    print(
        f"Warning: CSS file '{css_file_path}' not found. Skipping embedding.")
    return regex_match.group(0)


def embed_js_inline(regex_match):
    js_file_path = os.path.join("site", regex_match.group(1))
    if os.path.isfile(js_file_path):
        with open(js_file_path, "r", encoding="utf-8") as js_file:
            js_text = js_file.read()
        return f'<script>{js_text}</script>'
    print(f"Warning: JS file '{js_file_path}' not found. Skipping embedding.")
    return regex_match.group(0)


def process_html_content(html_content):
    html_content = re.sub(
        r'<link rel="stylesheet" href="([^"]+)">', embed_css_inline, html_content)
    html_content = re.sub(
        r'<script src="([^"]+)"></script>', embed_js_inline, html_content)
    html_content = re.sub(
        r'src="([^"]+)"', replace_src_with_base64, html_content)
    return minify_html.minify(
        html_content, minify_css=True, minify_js=True, do_not_minify_doctype=True,
        ensure_spec_compliant_unquoted_attribute_values=True, keep_spaces_between_attributes=True
    )


def process_html_file(input_file, output_file):
    with open(input_file, "r", encoding="utf-8") as input_handle:
        original_html = input_handle.read()
    modified_html = process_html_content(original_html)
    with open(output_file, "w", encoding="utf-8") as output_handle:
        output_handle.write(modified_html)
    print(f"Saved as {output_file}")


if __name__ == '__main__':
    argument_parser = argparse.ArgumentParser(
        description="Convert file references in HTML to base64 data URIs and embed stylesheets."
    )
    argument_parser.add_argument(
        "-i", "--input", dest="input_file", required=True, help="The input HTML file to process"
    )
    argument_parser.add_argument(
        "-o", "--output", dest="output_file", required=True,
        help="The output file where modified HTML will be saved"
    )
    args = argument_parser.parse_args()
    process_html_file(args.input_file, args.output_file)
