# `cbindgen` &emsp; [![Build Status]][travis] [![Latest Version]][crates.io]

[Build Status]: https://api.travis-ci.org/rlhunt/cbindgen.svg?branch=master
[travis]: https://travis-ci.org/rlhunt/cbindgen
[Latest Version]: https://img.shields.io/crates/v/cbindgen.svg
[crates.io]: https://crates.io/crates/cbindgen

This project can be used to generate C bindings for Rust code. It is currently being developed to support creating bindings for [WebRender](https://github.com/servo/webrender/), but has been designed to support any project.

## Features

  * Builds bindings for a crate, its mods, its dependent crates, and their mods
  * Only the necessary types for exposed functions are given bindings
  * Can specify annotations for controlling some aspects of binding
  * Generic structs can be exposed using `type IntFoo = Foo<i32>;`
  * Customizable formatting, can be used in C or C++ projects

## Use

### Command line

`cbindgen crate/ -o crate/bindings.h`

See `cbindgen --help` for more options.

### `build.rs`

`cbindgen` can also be used in build scripts. How this fits into compiling the native code depends on your project.

Here's an example build.rs script:
```rust
extern crate cbindgen;

use std::env;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();

    cbindgen::generate(crate_dir)
      .unwrap()
      .write_to_file("bindings.h");
}

```

## Configuration

There are some options that can be used to configure the binding generation. They can be specified by creating a `cbindgen.toml` with the options in the binding crate root or at a path manually specified through the command line. Alternatively, build scripts can specify them using `cbindgen::generate_with_config`.

Here is a description of the options available in a config.

```toml
# An optional string of text to output at the beginning of the generated file
header = <string>
# An optional string of text to output at the end of the generated file
trailer = <string>
# An optional name to use as an include guard
include_guard = <string>
# An optional string of text to output between major sections of the generated
# file as a warning against manual editing
autogen_warning = <string>
# Whether to include a comment with the version of cbindgen used to generate the
# file
include_version = <bool>
# An optional namespace to output around the generated bindings
namespace = <string>
# An optional list of namespaces to output around the generated bindings
namespaces = [<string>]
# The style to use for curly braces
braces = <curly>
# The desired length of a line to use when formatting lines
line_length = <integer>
# The amount of spaces in a tab
tab_width = <integer>
# The language to output bindings in
language = <language>

[parse]
# Whether to parse dependent crates and include their types in the generated
# bindings
parse_deps = <bool>
# A white list of crate names that are allowed to be parsed
include = [<string>]
# A black list of crate names that are not allowed to be parsed
exclude = [<string>]
# A list of crate names that should be run through `cargo expand` before
# parsing to expand any macros
expand = [<string>]

[fn]
# An optional prefix to put before every function declaration
prefix = <string>
# An optional postfix to put after any function declaration
postfix = <string>
# How to format function arguments
args = <layout>
# A rule to use to rename function argument names
rename_args = <rename-rule>

[struct]
# A rule to use to rename field names
rename_fields = <rename-rule>
# Whether to derive an operator== for all structs
derive_eq = <bool>
# Whether to derive an operator!= for all structs
derive_neq = <bool>
# Whether to derive an operator< for all structs
derive_lt = <bool>
# Whether to derive an operator<= for all structs
derive_lte = <bool>
# Whether to derive an operator> for all structs
derive_gt = <bool>
# Whether to derive an operator>= for all structs
derive_gte = <bool>

[enum]
# A rule to use to rename enum variants
rename_variants = <rename-rule>

# With

<curly> = ["SameLine" | "NextLine"]
<language> = ["C++" | "C"]
<layout> = ["Auto" |
            "Vertical" |
            "Horizontal"]
<rename-rule> = ["None" |
                 "GeckoCase" |
                 "LowerCase" |
                 "UpperCase" |
                 "PascalCase" |
                 "CamelCase" |
                 "SnakeCase" |
                 "ScreamingSnakeCase" |
                 "QualifiedScreamingSnakeCase"]
```

A full listing of options can be found in `src/bindgen/config.rs`.

## Examples

See `compile-tests/` for some examples of rust source that can be handled.

## How it works

1. All the structs, enums, type aliases, and functions that are representable in C are gathered
2. A dependency graph is built using the extern "C" functions as roots
    * This removes unneeded types from the bindings and sorts the structs that depend on each other
3. Some code generation is done to specialize generics that are specified as type aliases
4. The items are printed in dependency order in C syntax

## Future work

1. Better support for types with fully specified names
2. Support for generating a FFI interface for a Struct+Impl
3. ...
