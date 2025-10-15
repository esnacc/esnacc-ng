# Copilot Instructions for esnacc-ng

## Project Overview

eSNACC (Enhanced Sample Neufeld ASN C Compiler) is a comprehensive ASN.1 to C/C++ compiler that generates encoding/decoding routines for BER, DER, and PER rulesets. The project also has experimental support for TCL and Python bindings.

## Key Technologies and Architecture

- **Primary Languages**: C and C++ (compiler and runtime libraries)
- **Build System**: GNU Autotools (autoconf, automake, libtool)
- **Parser Generator**: Lex/Flex and Yacc/Bison
- **Testing Framework**: Built-in test suite with `make check`
- **Code Coverage**: LCOV integration via `--enable-code-coverage`
- **Additional Languages**: Python, TCL (experimental support)

## Development Workflow

### Building the Project

1. **Bootstrap**: Always run `./boot.sh` after fresh checkout to generate configure scripts
2. **Configure**: Run `./configure` with appropriate options (see `./configure --help`)
3. **Build**: Use `make` to compile the project
4. **Install**: Use `make install` to install binaries and libraries

### Testing and Quality Assurance

- **Basic Testing**: Always run `make check` before submitting changes
- **Code Coverage**: Use `./configure --enable-code-coverage && make check-code-coverage` for thorough testing
- **Clean Builds**: Ensure clean source tree when switching between debug/release builds

## Code Style and Standards

### C/C++ Guidelines

- Follow existing code style in the repository
- Use strict prototypes (`-Wstrict-prototypes -Wall` are enabled)
- Maintain compatibility with both C and C++ compilation
- Pay attention to memory management (especially for C code)
- Use appropriate ASN.1 naming conventions

### File Organization

- **Compiler Source**: Located in `compiler/` directory
- **C Runtime**: Located in `c-lib/` directory  
- **C++ Runtime**: Located in `cxx-lib/` directory
- **Examples**: Located in `c-examples/`, `cxx-examples/` directories
- **Documentation**: Located in `doc/` directory

## Licensing Requirements

**Critical**: This project has dual licensing:

- **Compiler Code**: Must be GPLv2+ licensed
- **Runtime Libraries**: Must be Public Domain

When contributing:
- Any compiler changes MUST be GPL v2+ compatible
- Any runtime library changes MUST be contributed to Public Domain
- Ensure you understand which part of the codebase you're modifying

## ASN.1 Specific Considerations

### Supported ASN.1 Features

- ASN.1 '90 syntax (including subtype notation)
- Multiple module compilation with IMPORTS/EXPORTS
- OBJECT IDENTIFIER, INTEGER, BOOLEAN value notation
- ANY DEFINED BY types using SNMP OBJECT-TYPE macro
- X.400 and SNMP macros

### Code Generation Targets

- C routines for BER encoding/decoding/printing/freeing
- C++ routines for BER encoding/decoding/printing
- Python routines (experimental)
- Type tables for runtime-driven encoding/decoding

## Contribution Guidelines

### Patch Submission

- Follow email-based workflow to dev@lists.esnacc.org
- Use `git format-patch` for proper patch formatting
- Subject format: `[PATCH n/m] AREA: summary`
- Include rationale, design description, and testing information
- Sign-off required (Developer's Certificate of Origin)

### Change Requirements

- **One logical change per patch**: Avoid grouping unrelated changes
- **No regressions**: Ensure clean builds and passing tests
- **Documentation updates**: Update docs when adding new features
- **Testing coverage**: Ensure new code is properly tested

## Common Development Tasks

### Adding New ASN.1 Features

1. Update parser (likely in `compiler/` with .y/.l files)
2. Update code generators for target languages
3. Add test cases in appropriate example directories
4. Update documentation if user-visible

### Runtime Library Changes

1. Consider impact on all supported languages (C, C++, Python, TCL)
2. Maintain API compatibility where possible
3. Update examples that demonstrate the feature
4. Remember: runtime changes must be Public Domain

### Build System Modifications

1. Update `Makefile.am` files as needed
2. Modify `configure.ac` for new dependencies or options
3. Run `./boot.sh` to regenerate build files
4. Test with different configure options

## Testing Strategy

### Comprehensive Testing

- Run full test suite: `make check`
- Test with code coverage: `make check-code-coverage`
- Test different build configurations (debug/release)
- Test cross-compilation if applicable
- Validate generated code compiles and runs correctly

### ASN.1 Specific Testing

- Test with various ASN.1 input files
- Verify correct encoding/decoding roundtrips
- Test edge cases and error conditions
- Validate compliance with ASN.1 standards

## Code Review Focus Areas

### Critical Areas for Review

1. **Memory Management**: Especially in C code, watch for leaks and double-frees
2. **Parser Changes**: Lex/Yacc modifications require careful testing
3. **Code Generation**: Ensure generated code is syntactically correct
4. **Cross-platform Compatibility**: Test on different systems
5. **License Compliance**: Verify appropriate licensing for changes

### Performance Considerations

- Generated code efficiency is important
- Minimize memory allocations in runtime libraries  
- Consider impact on compile time for large ASN.1 specifications
- Optimize common encoding/decoding paths

## Debugging and Troubleshooting

### Common Issues

- **Parse Errors**: Check ASN.1 syntax compliance
- **Generated Code Issues**: Verify code generator logic
- **Runtime Failures**: Check memory management and type handling
- **Build Failures**: Ensure all dependencies are available

### Debugging Tools

- Use appropriate debuggers (gdb for C/C++)
- Enable code coverage for test analysis
- Use valgrind for memory issue detection
- Check generated code manually when needed

## Release and Maintenance

### Version Management

- Version defined in `configure.ac` using m4 macros
- Interface versioning for libraries using libtool
- Maintain backward compatibility when possible
- Document breaking changes clearly

This guidance should help you contribute effectively to the eSNACC project while maintaining code quality and project standards.