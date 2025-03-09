# Function Tracing Tool using Intel Pin

## Overview

This project is a function tracing tool built using the Intel Pin dynamic binary instrumentation framework. It intercepts and records function calls in a binary executable, storing them in a text file for analysis.

## Features

- Intercepts function calls in the main executable.
- Excludes specific function names based on predefined patterns.
- Logs function call sequences to `traced_functions.txt`.
- Supports multithreaded execution.
- Uses Intel Pin as the instrumentation framework.

## Installation and Setup

To install and set up the tool, run the provided Bash script:

```bash
chmod +x setup.sh
./setup.sh
```

This script will:

1. Download and extract Intel Pin.
2. Set up compiler environment variables.
3. Build the project using `make`.

If any errors occur during installation, they will be displayed in red.

## Usage

Once setup is complete, you can use the function tracing tool as follows:

```bash
./pin/pin -t obj-intel64/traceF.so -- <program> <arg1> <arg2> ... <argn>
```

### Example

To trace a binary executable `target_app`:

```bash
./pin/pin -t obj-intel64/traceF.so -- ./target_app
```

## Output

After running the tool, the traced function names will be stored in `traced_functions.txt`.

## Code Structure

- `traceF.cpp`: Implements the function tracing logic.
- `setup.sh`: Automates the installation and setup process.
- `Makefile`: Compiles the tool using `g++` and Intel Pin.
- `traced_functions.txt`: Stores logged function calls.

## Dependencies

- Intel Pin 3.28
- GCC/G++
- Linux Environment

## Notes

- The tool only traces functions from the main executable, excluding shared libraries.
- If you encounter any issues, verify that Intel Pin is correctly extracted and that `make` builds successfully.

## License

This project is released under the MIT License.

## Contact

For any issues or contributions, feel free to submit a pull request or open an issue.