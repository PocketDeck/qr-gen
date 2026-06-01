# QR Code Generator

A lightweight QR code generator for text input.

## Features

- Generate QR codes from text input
- Numeric, Alphanumeric, Byte mode encoding
- Support for multiple error correction levels (L, M, Q, H)
- Supported output formats:
    - PBM
    - SVG
    - terminal output
- Simple command-line interface
- Pure C implementation with no external dependencies
- Supports standard QR code versions only (no Micro QR support)

## Prerequisites

- C compiler (GCC or Clang)
- Make

## Quickstart

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd qr-gen
   ```

2. Build the executable:
   ```bash
   make build/release/qr-gen
   ```
   This will create the `qr-gen` executable in the `build/release` directory.

## Usage

```bash
./build/release/qr-gen "Your text here" [error_correction [out_format]]
```

### Error Correction Levels

- `L` - Low (7% of codewords can be restored)
- `M` - Medium (15% of codewords can be restored) - **Default**
- `Q` - Quartile (25% of codewords can be restored)
- `H` - High (30% of codewords can be restored)

### Output Format

- `PBM` - Portable Bitmap
- `SVG` - Scalable Vector Graphics
- `TERM` - ANSI Codes for terminal output

The program produces the QR code in the specified format to standard output (stdout). You can redirect the output to a file:

```bash
./build/release/qr-gen "Your text here" M SVG > qrcode.svg
```

### Examples

Generate a QR code with default error correction (M):
```bash
./build/release/qr-gen "Hello, World!"
```

Generate a QR code with high error correction (using the `run` rule):
```bash
make run TEXT="Important Data" ECL=H OUTFMT=PBM
```

## Running Tests

The project includes unit tests to verify the functionality of core components. Oracle testing requires a POSIX-complient system and the following tools:
- `curl`
- `imagemagick`
- `awk`
- `netpbm`

To run the tests:

```bash
make -s test
```

## Project Structure

```
qr/              - Main source code
├─ ecc.[ch]      - Error correction coding
├─ enc.[ch]      - Data encoding
├─ info.[ch]     - Format and version info
├─ mask.[ch]     - Mask pattern generation
├─ matrix.[ch]   - QR code matrix operations
├─ patterns.[ch] - QR code patterns and alignment
├─ qr.[ch]       - Main QR code functionality
├─ types.h       - Common type definitions
├─ tables.h      - Lookup table definitions
└─ main.c        - Command-line interface

test/            - Unit tests
```
