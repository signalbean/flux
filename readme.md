<div align="center">

# FLUX

CLI for downloading and uploading files with resume support.

</div>

## Features

- Download files from any URL with resume
- Upload files to remote servers
- Cross-platform

## Dependencies

- GCC compiler
- libcurl development headers

## Build

```bash
make all # Standard build
```

## Usage

```bash
# Download files
flux download https://example.com/file.zip
flux -d https://example.com/file.zip custom-name.zip

# Upload files
flux upload file.txt https://example.com/upload
```

<div align="center">

## License

MIT

</div>
