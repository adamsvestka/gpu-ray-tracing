## Makefile Structure

The make process of this project uses a waterfall structure with multiple makefiles located in different directories. Each makefile is responsible for building the files in its associated directory, and they are eventually called by the root makefile.

### How to Run Make

You should only interact with the makefile located in the root directory of the project. To run a specific target, you can execute the `make` command followed by the target name. If you're in a subdirectory, you should use the `-C` flag followed by the appropriate path to the root directory.

For example, if you are in the root directory, you can simply run:

```sh
make <target>
```

If you are in a subdirectory, you should run:

```sh
make -C <path_to_root_directory> <target>
```

Replace `<target>` with one of the available targets and `<path_to_root_directory>` with the relative path to the root directory.

### Available Targets

- `help`: Shows help information about the project and available targets.
- `build`: Builds the project (default). It includes:
  - `build-deps`: Builds the dependencies.
  - `build-app`: Builds the application.
  - `build-tests`: Builds the tests.
- `copy-headers`: Copies the headers.
- `copy-assets`: Copies the assets.
- `clean`: Cleans the project.
- `clean-all`: Cleans the project and dependencies.
- `download`: Downloads the dependencies.
- `run`: Runs the project.
- `test`: Runs the tests. It includes:
  - `test-main`: Runs the unit tests.
  - `test-shaders`: Runs the shader tests.

### Example Make Commands

Here are some examples of using make commands:

1. Build the project:

   In the root directory, run:

   ```sh
   make build
   ```

2. Clean the project:

   In the root directory, run:

   ```sh
   make clean
   ```

3. Run the project:

   In the root directory, run:

   ```sh
   make run
   ```

4. Show help information:

   From anywhere, run:

   ```sh
   make help
   ```

You can only interact with the root makefile, as the other makefiles are designed to be called by the root makefile only and will not work on their own.
