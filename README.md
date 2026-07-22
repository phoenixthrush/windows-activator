# Windows Activation Toolkit

A small Windows desktop interface for Windows, Office, and Visual Studio tools.

## Build

~~~bash
make
~~~

For a Windows cross-build:

~~~bash
make cross
~~~

Run the small build-helper tests with `make test`.

The build checks dependencies, bundles the web interface into the executable,
and places finished binaries under `build/bin`.

## Project layout

- `src` contains the small C application and product modules.
- `site` contains the interface shown inside the application.
- `scripts` contains readable build and packaging helpers.
- `tests` protects the HTML and C embedding steps.

## Interface

- Click a product button to run its native action.
- Press Space to play or pause music.
- Use Left/Right to seek and Up/Down to change volume.
- Image and audio credits open their original sources.

Windows and Office actions require administrator privileges. Windows activation
also requires an internet connection.

## Credits

- [GamersOsState](https://github.com/asdcorp/GamersOsState/)
- [ohook](https://github.com/asdcorp/ohook/)

MIT licensed.
