# Number Base Converter

This program allows users to convert numbers between binary, hexadecimal, and decimal formats. Additionally, it provides 1's complement and 2's complement representations for the given values.

## Features

- **Input Formats:** Accepts binary, hexadecimal (not prefixed with `0x`), and decimal inputs.
- **Conversions:** Converts input numbers to:
  - Binary
  - Hexadecimal
  - Decimal
- **Complements:** Computes and displays:
  - 1's Complement
  - 2's Complement
- **Powers Representation:** Displays powers of 2 for the given decimal values.

## Getting Started

### Prerequisites

- A TI-84 Plus CE calculator or compatible environment for running the code.
- [CE Programming Toolchain](https://github.com/CE-Programming/toolchain) for compiling the source code.

### Compilation

1. Download the [CE Programming Toolchain](https://github.com/CE-Programming/toolchain) repository.
2. Open cedev.bat
3. Compile the source code from there, see examples if you don't get it.

### Usage

1. Run the program.
2. Select the input format:
   - **1:** Binary to others
   - **2:** Hex to others
   - **3:** Decimal to others
   - **4:** Exit
3. Enter the desired number in the chosen format.
4. View the results, including the original value, conversions, and complements.

## Validation

The program includes validation checks for:
- Binary input (only `0` and `1`).
- Hexadecimal input (digits and letters `A`-`F`).
- Decimal input (only digits).

## License

This project is open source. Feel free to use and modify the code as needed.
