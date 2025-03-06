import re
import base64
import os
import argparse
import logging

try:
    import minify_html
    MINIFY_AVAILABLE = True
except ImportError:
    MINIFY_AVAILABLE = False

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)
SITE_DIR = "site"


def convert_file_to_base64(file_path):
    if not os.path.isfile(file_path):
        return None
    with open(file_path, "rb") as f:
        return base64.b64encode(f.read()).decode("utf-8")


def get_mime_type(ext):
    ext = ext.lower()
    if ext in {"png", "jpg", "jpeg", "gif", "webp"}:
        return f"image/{ext}"
    if ext == "svg":
        return "image/svg+xml"
    if ext == "mp3":
        return "audio/mpeg"
    if ext == "ogg":
        return "audio/ogg"
    return "application/octet-stream"


def get_data_uri(file_path):
    b64 = convert_file_to_base64(file_path)
    if b64:
        ext = os.path.splitext(file_path)[1][1:]
        mime = get_mime_type(ext)
        return f"data:{mime};base64,{b64}"
    logger.warning(f"File '{file_path}' not found.")
    return None


def replace_src_with_data_uri(match):
    path = match.group(1)
    file_path = os.path.join(SITE_DIR, path)
    data_uri = get_data_uri(file_path)
    if data_uri:
        return f'src="{data_uri}"'
    return match.group(0)


def replace_image_href(match):
    prefix = match.group(1)
    path = match.group(2)
    file_path = os.path.join(SITE_DIR, path)
    data_uri = get_data_uri(file_path)
    if data_uri:
        return f'{prefix}href="{data_uri}"'
    return match.group(0)


def embed_css_inline(match):
    path = match.group(1)
    file_path = os.path.join(SITE_DIR, path)
    if os.path.isfile(file_path):
        with open(file_path, "r", encoding="utf-8") as f:
            css = f.read()
        return f'<style>{css}</style>'
    logger.warning(f"CSS file '{file_path}' not found.")
    return match.group(0)


def embed_js_inline(match):
    path = match.group(1)
    file_path = os.path.join(SITE_DIR, path)
    if os.path.isfile(file_path):
        with open(file_path, "r", encoding="utf-8") as f:
            js = f.read()
        return f'<script>{js}</script>'
    logger.warning(f"JS file '{file_path}' not found.")
    return match.group(0)


def process_html_content(html):
    html = re.sub(r'<link rel="stylesheet" href="([^"]+)">', embed_css_inline, html)
    html = re.sub(r'<script src="([^"]+)"></script>', embed_js_inline, html)
    html = re.sub(r'(<image[^>]*?)href="([^"]+)"', replace_image_href, html)
    html = re.sub(r'src="([^"]+)"', replace_src_with_data_uri, html)
    if MINIFY_AVAILABLE:
        html = minify_html.minify(
            html, minify_css=True, minify_js=True, do_not_minify_doctype=True,
            ensure_spec_compliant_unquoted_attribute_values=True, keep_spaces_between_attributes=True
        )
    else:
        logger.info("Minify_html not installed. Skipping minification.")
    return html


def process_html_file(input_file, output_file):
    with open(input_file, "r", encoding="utf-8") as f:
        html = f.read()
    modified = process_html_content(html)
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(modified)
    logger.info(f"Saved as {output_file}")


def main():
    parser = argparse.ArgumentParser(description="Embed resources in HTML as base64 and inline CSS/JS.")
    parser.add_argument("-i", "--input", dest="input_file", required=True)
    parser.add_argument("-o", "--output", dest="output_file", required=True)
    args = parser.parse_args()
    process_html_file(args.input_file, args.output_file)


if __name__ == '__main__':
    main()
