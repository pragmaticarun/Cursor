#!/bin/bash

# C Code Auto-Formatter Script
# Provides multiple formatting options and code quality tools

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SRCDIR="src"
TESTDIR="tests"
INCDIR="include"
EXAMPLEDIR="examples"
FORMATDIR="formatted"

# Print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if tool is available
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        print_warning "$1 not found, skipping..."
        return 1
    fi
    return 0
}

# Format with clang-format
format_clang() {
    print_status "Formatting with clang-format..."
    
    if check_tool clang-format; then
        # Create formatted directory
        mkdir -p "$FORMATDIR"
        
        # Format source files
        for file in "$SRCDIR"/*.c; do
            if [[ -f "$file" ]]; then
                local basename=$(basename "$file")
                clang-format "$file" > "$FORMATDIR/$basename"
                print_success "Formatted $basename"
            fi
        done
        
        # Format test files
        for file in "$TESTDIR"/*.c; do
            if [[ -f "$file" ]]; then
                local basename=$(basename "$file")
                clang-format "$file" > "$FORMATDIR/test_$basename"
                print_success "Formatted test_$basename"
            fi
        done
        
        # Format header files
        for file in "$INCDIR"/*.h; do
            if [[ -f "$file" ]]; then
                local basename=$(basename "$file")
                clang-format "$file" > "$FORMATDIR/$basename"
                print_success "Formatted $basename"
            fi
        done
        
        # Format example files
        for file in "$EXAMPLEDIR"/*.c; do
            if [[ -f "$file" ]]; then
                local basename=$(basename "$file")
                clang-format "$file" > "$FORMATDIR/example_$basename"
                print_success "Formatted example_$basename"
            fi
        done
    fi
}

# Format with indent
format_indent() {
    print_status "Formatting with indent..."
    
    if check_tool indent; then
        # Create formatted directory
        mkdir -p "${FORMATDIR}_indent"
        
        # Format source files
        for file in "$SRCDIR"/*.c; do
            if [[ -f "$file" ]]; then
                local basename=$(basename "$file")
                indent -kr -i4 -ts4 -nut -l80 -nhnl "$file" -o "${FORMATDIR}_indent/$basename"
                print_success "Formatted $basename with indent"
            fi
        done
    fi
}

# Format with astyle
format_astyle() {
    print_status "Formatting with astyle..."
    
    if check_tool astyle; then
        # Create formatted directory
        mkdir -p "${FORMATDIR}_astyle"
        
        # Format source files
        for file in "$SRCDIR"/*.c; do
            if [[ -f "$file" ]]; then
                local basename=$(basename "$file")
                astyle --style=kr --indent=spaces=4 --lineend=linux --pad-oper --pad-header --unpad-paren --align-pointer=name --align-reference=name "$file" --suffix=none --stdout > "${FORMATDIR}_astyle/$basename"
                print_success "Formatted $basename with astyle"
            fi
        done
    fi
}

# Check code style with cppcheck
check_cppcheck() {
    print_status "Running cppcheck..."
    
    if check_tool cppcheck; then
        cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem "$SRCDIR" "$TESTDIR" "$INCDIR" 2>&1 | tee cppcheck_report.txt
        print_success "Cppcheck report saved to cppcheck_report.txt"
    fi
}

# Check code style with clang-tidy
check_clang_tidy() {
    print_status "Running clang-tidy..."
    
    if check_tool clang-tidy; then
        # Create compile_commands.json for clang-tidy
        echo '[' > compile_commands.json
        echo '  {' >> compile_commands.json
        echo '    "directory": "'$(pwd)'",' >> compile_commands.json
        echo '    "command": "gcc -std=c11 -Wall -Wextra -O2 -Iinclude -c src/01_types.c",' >> compile_commands.json
        echo '    "file": "src/01_types.c"' >> compile_commands.json
        echo '  }' >> compile_commands.json
        echo ']' >> compile_commands.json
        
        clang-tidy src/*.c -- -std=c11 -Wall -Wextra -O2 -Iinclude 2>&1 | tee clang_tidy_report.txt
        print_success "Clang-tidy report saved to clang_tidy_report.txt"
    fi
}

# Check code style with splint
check_splint() {
    print_status "Running splint..."
    
    if check_tool splint; then
        splint +posixlib -warnposix -preproc -redef -warn -hints "$SRCDIR"/*.c 2>&1 | tee splint_report.txt
        print_success "Splint report saved to splint_report.txt"
    fi
}

# Generate code metrics
generate_metrics() {
    print_status "Generating code metrics..."
    
    # Count lines of code
    local total_lines=$(find "$SRCDIR" "$TESTDIR" "$INCDIR" "$EXAMPLEDIR" -name "*.c" -o -name "*.h" | xargs wc -l | tail -1 | awk '{print $1}')
    local source_lines=$(find "$SRCDIR" -name "*.c" | xargs wc -l | tail -1 | awk '{print $1}')
    local test_lines=$(find "$TESTDIR" -name "*.c" | xargs wc -l | tail -1 | awk '{print $1}')
    local header_lines=$(find "$INCDIR" -name "*.h" | xargs wc -l | tail -1 | awk '{print $1}')
    local example_lines=$(find "$EXAMPLEDIR" -name "*.c" | xargs wc -l | tail -1 | awk '{print $1}')
    
    # Count functions
    local function_count=$(find "$SRCDIR" -name "*.c" | xargs grep -c "^[a-zA-Z_][a-zA-Z0-9_]*.*(" | awk '{sum += $1} END {print sum}')
    
    # Count files
    local file_count=$(find "$SRCDIR" "$TESTDIR" "$INCDIR" "$EXAMPLEDIR" -name "*.c" -o -name "*.h" | wc -l)
    
    # Generate report
    cat > code_metrics.txt << EOF
C Code Metrics Report
====================

Total Lines of Code: $total_lines
  Source Files: $source_lines lines
  Test Files: $test_lines lines
  Header Files: $header_lines lines
  Example Files: $example_lines lines

Total Files: $file_count
Total Functions: $function_count

Average Lines per File: $((total_lines / file_count))
Average Lines per Function: $((total_lines / function_count))

File Breakdown:
$(find "$SRCDIR" "$TESTDIR" "$INCDIR" "$EXAMPLEDIR" -name "*.c" -o -name "*.h" | xargs wc -l | sort -nr)
EOF
    
    print_success "Code metrics saved to code_metrics.txt"
}

# Create custom formatting rules
create_formatting_rules() {
    print_status "Creating custom formatting rules..."
    
    # Create .clang-format with custom rules
    cat > .clang-format << 'EOF'
---
Language: Cpp
BasedOnStyle: Google
IndentWidth: 4
TabWidth: 4
UseTab: Never
ColumnLimit: 100
AccessModifierOffset: -2
AlignAfterOpenBracket: Align
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
AlignOperands: true
AlignTrailingComments: true
AllowAllParametersOfDeclarationOnNextLine: true
AllowShortBlocksOnASingleLine: false
AllowShortCaseLabelsOnASingleLine: false
AllowShortFunctionsOnASingleLine: None
AllowShortIfStatementsOnASingleLine: false
AllowShortLoopsOnASingleLine: false
AlwaysBreakAfterReturnType: None
AlwaysBreakBeforeMultilineStrings: false
AlwaysBreakTemplateDeclarations: Yes
BinPackArguments: true
BinPackParameters: true
BraceWrapping:
  AfterClass: false
  AfterControlStatement: false
  AfterEnum: false
  AfterFunction: false
  AfterNamespace: false
  AfterStruct: false
  AfterUnion: false
  BeforeCatch: false
  BeforeElse: false
  IndentBraces: false
BreakBeforeBinaryOperators: None
BreakBeforeBraces: Attach
BreakBeforeTernaryOperators: true
BreakConstructorInitializersBeforeComma: false
BreakAfterJavaFieldAnnotations: false
BreakStringLiterals: true
ConstructorInitializerAllOnOneLineOrOnePerLine: false
ConstructorInitializerIndentWidth: 4
ContinuationIndentWidth: 4
Cpp11BracedListStyle: true
DerivePointerAlignment: false
DisableFormat: false
ExperimentalAutoDetectBinPacking: false
FixNamespaceComments: true
IncludeBlocks: Preserve
IndentCaseLabels: false
IndentPPDirectives: None
IndentWrappedFunctionNames: false
JavaScriptQuotes: Leave
JavaScriptWrapImports: true
KeepEmptyLinesAtTheStartOfBlocks: false
MacroBlockBegin: ''
MacroBlockEnd: ''
MaxEmptyLinesToKeep: 1
NamespaceIndentation: None
PenaltyBreakAssignment: 2
PenaltyBreakBeforeFirstCallParameter: 1
PenaltyBreakComment: 300
PenaltyBreakFirstLessLess: 120
PenaltyBreakString: 1000
PenaltyExcessCharacter: 1000000
PenaltyReturnTypeOnItsOwnLine: 200
PointerAlignment: Left
ReflowComments: true
SortIncludes: true
SortUsingDeclarations: true
SpaceAfterCStyleCast: false
SpaceAfterTemplateKeyword: true
SpaceBeforeAssignmentOperators: true
SpaceBeforeParens: ControlStatements
SpaceInEmptyParentheses: false
SpacesBeforeTrailingComments: 2
SpacesInAngles: false
SpacesInContainerLiterals: true
SpacesInCStyleCastParentheses: false
SpacesInParentheses: false
SpacesInSquareBrackets: false
Standard: Cpp11
EOF
    
    print_success "Custom formatting rules created"
}

# Clean up generated files
cleanup() {
    print_status "Cleaning up generated files..."
    rm -rf "$FORMATDIR" "${FORMATDIR}_indent" "${FORMATDIR}_astyle"
    rm -f compile_commands.json
    print_success "Cleanup complete"
}

# Show help
show_help() {
    echo "C Code Auto-Formatter Script"
    echo "============================"
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  clang       - Format with clang-format"
    echo "  indent      - Format with indent"
    echo "  astyle      - Format with astyle"
    echo "  cppcheck    - Run cppcheck static analysis"
    echo "  clang-tidy  - Run clang-tidy static analysis"
    echo "  splint      - Run splint static analysis"
    echo "  metrics     - Generate code metrics"
    echo "  rules       - Create custom formatting rules"
    echo "  all         - Run all formatting and analysis tools"
    echo "  clean       - Clean up generated files"
    echo "  help        - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 clang      # Format with clang-format"
    echo "  $0 all        # Run all tools"
    echo "  $0 clean      # Clean up"
}

# Main function
main() {
    case "${1:-help}" in
        "clang")
            format_clang
            ;;
        "indent")
            format_indent
            ;;
        "astyle")
            format_astyle
            ;;
        "cppcheck")
            check_cppcheck
            ;;
        "clang-tidy")
            check_clang_tidy
            ;;
        "splint")
            check_splint
            ;;
        "metrics")
            generate_metrics
            ;;
        "rules")
            create_formatting_rules
            ;;
        "all")
            create_formatting_rules
            format_clang
            format_indent
            format_astyle
            check_cppcheck
            check_clang_tidy
            check_splint
            generate_metrics
            ;;
        "clean")
            cleanup
            ;;
        "help"|"-h"|"--help")
            show_help
            ;;
        *)
            print_error "Unknown command: $1"
            show_help
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@"
