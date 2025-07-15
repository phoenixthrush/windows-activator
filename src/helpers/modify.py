import re
import base64
import mimetypes
import argparse
from pathlib import Path


def read_file(path, mode="r", encoding="utf-8"):
    # Reads file as text or bytes, returns None on error
    try:
        return Path(path).read_text(encoding=encoding) if "b" not in mode else Path(path).read_bytes()
    except Exception:
        return None


def base64_data_uri(file_path):
    # Converts a file to base64 data URI with correct MIME type
    data = read_file(file_path, mode="rb")
    if not data:
        return None
    mime, _ = mimetypes.guess_type(file_path.name)
    mime = mime or "application/octet-stream"
    return f"data:{mime};base64,{base64.b64encode(data).decode()}"


def inline_file(file_path, tag, site_dir):
    # Embeds a local CSS or JS file as <style> or <script> tag
    content = read_file(file_path)
    if not content:
        return None
    if tag == "style":
        # Replace url(...) in CSS with base64-encoded data URIs
        content = re.sub(r'url\(["\']?([^)"\']+)["\']?\)', lambda m: css_url_to_base64(m[1], site_dir), content)
    return f"<{tag}>{content}</{tag}>"


def css_url_to_base64(path, site_dir):
    # Converts a relative CSS url(...) to base64 data URI
    uri = base64_data_uri(site_dir / path)
    return f'url("{uri}")' if uri else f'url("{path}")'


def inline_css_file(file_path, site_dir, processed_files=None, wrap_in_style=True):
    if processed_files is None:
        processed_files = set()
    file_path = file_path.resolve()
    if file_path in processed_files or not file_path.exists():
        return None
    processed_files.add(file_path)

    content = read_file(file_path)
    if not content:
        return None

    # Inline @import statements (handle url(...) and quoted files)
    def import_replacer(import_path):
        import_path = import_path.strip(' "\'')
        import_file = (file_path.parent / import_path).resolve()
        # Recursively process imports without wrapping in <style> tags
        inlined_content = inline_css_file(import_file, site_dir, processed_files, wrap_in_style=False)
        return inlined_content or ''

    # Regex to catch @import url(...) and @import '...' or "..."
    content = re.sub(
        r'@import\s+(?:url\(([^)]+)\)|["\']([^"\']+)["\']);',
        lambda m: import_replacer(m.group(1) or m.group(2)),
        content
    )

    # Replace url(...) with base64
    content = re.sub(r'url\(["\']?([^)"\']+)["\']?\)', lambda m: css_url_to_base64(m[1], file_path.parent), content)

    return f"<style>{content}</style>" if wrap_in_style else content



def process_html(html, site_dir):
    # Replace <link rel="stylesheet" href="..."> with inline <style>
    html = re.sub(r'<link rel="stylesheet" href="([^"]+)">',
                  lambda m: inline_css_file(site_dir / m[1], site_dir) or m[0], html)

    # Replace <script src="..."></script> with inline <script>
    html = re.sub(r'<script src="([^"]+)"></script>',
                  lambda m: inline_file(site_dir / m[1], "script", site_dir) or m[0], html)

    # Replace <image ... href="..."> with base64 data URI
    html = re.sub(r'(<image[^>]*?)href="([^"]+)"',
                  lambda m: f'{m[1]}href="{base64_data_uri(site_dir / m[2])}"'
                  if (uri := base64_data_uri(site_dir / m[2])) else m[0], html)

    # Replace src="..." (img, audio, video) with base64 data URI
    html = re.sub(r'src="([^"]+)"',
                  lambda m: f'src="{base64_data_uri(site_dir / m[1])}"'
                  if (uri := base64_data_uri(site_dir / m[1])) else m[0], html)

    # Replace <source src="..."> inside <video>/<audio> with base64
    html = re.sub(r'<source\s+([^>]*?)src="([^"]+)"',
                  lambda m: f'<source {m[1]}src="{base64_data_uri(site_dir / m[2])}"'
                  if (uri := base64_data_uri(site_dir / m[2])) else m[0], html)

    # Replace <object data="..."> (e.g. PDF, SVG) with base64
    html = re.sub(r'<object\s+([^>]*?)data="([^"]+)"',
                  lambda m: f'<object {m[1]}data="{base64_data_uri(site_dir / m[2])}"'
                  if (uri := base64_data_uri(site_dir / m[2])) else m[0], html)

    # Replace <img srcset="..." src="..."> with just base64 src
    html = re.sub(r'<img[^>]*\s+srcset="[^"]*"\s+src="([^"]+)"',
                  lambda m: f'<img src="{base64_data_uri(site_dir / m[1])}"'
                  if (uri := base64_data_uri(site_dir / m[1])) else m[0], html)

    return html


def minify_html(html):
    # Remove HTML comments
    html = re.sub(r'<!--.*?-->', '', html, flags=re.DOTALL)
    # Remove newlines and whitespace between tags
    html = re.sub(r'>\s+<', '><', html)
    # Remove newlines within the entire HTML (this handles base64 data with newlines)
    html = re.sub(r'\n\s*', '', html)
    # Collapse multiple spaces to one
    html = re.sub(r'\s{2,}', ' ', html)
    # Strip leading/trailing whitespace
    return html.strip()


def main():
    # Parse input/output file paths
    p = argparse.ArgumentParser()
    p.add_argument("-i", "--input", required=True)
    p.add_argument("-o", "--output", required=True)
    a = p.parse_args()

    # Use input HTML's directory as base path for resources
    site_dir = Path(a.input).resolve().parent

    html = read_file(a.input)
    if html:
        out = process_html(html, site_dir)
        out = minify_html(out)
        Path(a.output).write_text(out, encoding="utf-8")


if __name__ == "__main__":
    main()