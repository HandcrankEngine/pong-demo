# Handcrank Engine Demo Template

## Dependencies

```bash
brew install cmake
brew install clang-format
brew install SDL2
brew install SDL2_image
brew install SDL2_ttf
```

## Build

### Native

Using CMake

```bash
cmake .
make
```

or using g++

```bash
./bin/build.sh
```

## Editor Setup

### VS Code

Make sure the paths are correct in `.vscode/c_cpp_properties.json`.

### zed

Make sure the paths are correct in `.clangd`.

### Sublime Text

Via Package Control, install `LSP` and `LSP-clangd`.
