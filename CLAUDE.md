# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Architecture

Sexpander is a JUCE audio plugin (VST) that controls audio levels according to sidechain bus.

**Core Components:**
- `PluginProcessor` - Main plugin processor handling audio
- `PluginEditor` - GUI with 2x6 grid layout

**File Organization:**
- Headers in `include/`
- Implementations in `source/`
- JUCE framework in `libs/juce` (as a gitmodule)

## Code Style

- No space between method/function names and parentheses: `foo(x)` not `foo (x)`
- Member variables use underscore suffix: `audioProcessor_` not `audioProcessor`
- Use JUCE naming conventions (camelCase for methods, PascalCase for classes)
- Split interface (header) and implementation (cpp)
- Each class should have its own files
- Trivial methods should stay in header

## Commit Messages

- Start with capital letter
- No trailing period
- Use imperative mood: `Fix bug` not `Fixed bug` or `Fixes bug`
- Keep concise (ideally under 50 characters)
