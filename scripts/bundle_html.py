"""Build the self-contained HTML page embedded in the desktop executable."""

import argparse
import base64
import mimetypes
import re
from pathlib import Path


def data_uri(path: Path):
    """Return a data URI for an existing local file."""
    if not path.is_file():
        return None

    mime_type = mimetypes.guess_type(path.name)[0] or "application/octet-stream"
    encoded = base64.b64encode(path.read_bytes()).decode("ascii")
    return f"data:{mime_type};base64,{encoded}"


def inline_css(path: Path, processed=None):
    """Inline one stylesheet, including its local imports and assets."""
    path = path.resolve()
    processed = processed or set()

    if path in processed or not path.is_file():
        return None
    processed.add(path)

    css = path.read_text(encoding="utf-8")

    def replace_import(match):
        imported = (path.parent / (match.group(1) or match.group(2))).resolve()
        return inline_css(imported, processed) or ""

    css = re.sub(
        r'@import\s+(?:url\(["\']?([^)"\']+)["\']?\)|["\']([^"\']+)["\']);',
        replace_import,
        css,
    )

    def replace_url(match):
        asset_path = (path.parent / match.group(1)).resolve()
        return f'url("{data_uri(asset_path) or match.group(1)}")'

    return re.sub(r'url\(["\']?([^)"\']+)["\']?\)', replace_url, css)


def inline_page(html: str, site_directory: Path) -> str:
    """Replace local CSS, JavaScript, and media references with embedded data."""

    def replace_stylesheet(match):
        css = inline_css(site_directory / match.group(1))
        return f"<style>{css}</style>" if css is not None else match.group(0)

    html = re.sub(
        r'<link\s+rel="stylesheet"\s+href="([^"]+)"\s*/?>',
        replace_stylesheet,
        html,
    )

    def replace_script(match):
        script_path = site_directory / match.group(1)
        if not script_path.is_file():
            return match.group(0)
        script = script_path.read_text(encoding="utf-8")
        return f"<script>{script}</script>"

    # Extra attributes such as "defer" are deliberately accepted.
    html = re.sub(
        r'<script\s+src="([^"]+)"[^>]*>\s*</script>',
        replace_script,
        html,
    )

    def replace_asset(match):
        uri = data_uri(site_directory / match.group(2))
        if uri is None:
            return match.group(0)
        return f'{match.group(1)}="{uri}"'

    # Script and stylesheet tags were handled above. Remaining local src/href/data
    # attributes belong to images, audio, video, SVG images, or objects.
    return re.sub(r'\b(src|href|data)="([^"]+)"', replace_asset, html)


def tidy_html(html: str) -> str:
    """Remove safe, structural whitespace without changing CSS or JavaScript."""
    html = re.sub(r"<!--.*?-->", "", html, flags=re.DOTALL)
    return re.sub(r">\s+<", "><", html).strip()


def build(input_path: Path, output_path: Path):
    site_directory = input_path.resolve().parent
    html = input_path.read_text(encoding="utf-8")
    bundled = tidy_html(inline_page(html, site_directory))

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(bundled, encoding="utf-8")


def parse_args():
    parser = argparse.ArgumentParser(
        description="Bundle a web page and its local assets into one HTML file."
    )
    parser.add_argument("-i", "--input", type=Path, required=True)
    parser.add_argument("-o", "--output", type=Path, required=True)
    return parser.parse_args()


def main():
    args = parse_args()
    build(args.input, args.output)


if __name__ == "__main__":
    main()
