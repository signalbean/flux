# FLUX

CLI for downloading and uploading files with resume support.

## Features

- Download files from any URL with resume
- Upload files to remote servers
- Cross-platform

## Dependencies

- GCC compiler
- libcurl development headers

## Build

```bash
make           # Standard build
make release   # Optimized build (smaller binary)
```

## Usage

```bash
# Download files
flux download https://example.com/file.zip
flux -d https://example.com/file.zip custom-name.zip

# Upload files
flux upload file.txt https://example.com/upload
```

## License

MIT
