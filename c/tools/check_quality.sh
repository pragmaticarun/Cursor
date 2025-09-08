#!/bin/bash

# C Code Quality Checker Script
# Comprehensive code quality analysis and reporting

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
REPORTDIR="quality_reports"
CC="gcc"
CFLAGS="-std=c11 -Wall -Wextra -Werror -pedantic -O2 -Iinclude"

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

# Create report directory
create_report_dir() {
    mkdir -p "$REPORTDIR"
}

# Check if tool is available
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        print_warning "$1 not found, skipping..."
        return 1
    fi
    return 0
}

# Compile with strict warnings
check_compilation() {
    print_status "Checking compilation with strict warnings..."
    
    local error_count=0
    local warning_count=0
    
    # Check source files
    for file in "$SRCDIR"/*.c; do
        if [[ -f "$file" ]]; then
            local basename=$(basename "$file")
            print_status "Compiling $basename..."
            
            if $CC $CFLAGS -c "$file" -o /tmp/test_compile.o 2>&1 | tee "$REPORTDIR/compile_$basename.log"; then
                print_success "Compiled $basename successfully"
            else
                print_error "Failed to compile $basename"
                error_count=$((error_count + 1))
            fi
            
            # Count warnings
            local warnings=$(grep -c "warning:" "$REPORTDIR/compile_$basename.log" 2>/dev/null || echo "0")
            warning_count=$((warning_count + warnings))
            
            rm -f /tmp/test_compile.o
        fi
    done
    
    # Check test files
    for file in "$TESTDIR"/*.c; do
        if [[ -f "$file" ]]; then
            local basename=$(basename "$file")
            print_status "Compiling test $basename..."
            
            if $CC $CFLAGS -c "$file" -o /tmp/test_compile.o 2>&1 | tee "$REPORTDIR/compile_test_$basename.log"; then
                print_success "Compiled test $basename successfully"
            else
                print_error "Failed to compile test $basename"
                error_count=$((error_count + 1))
            fi
            
            # Count warnings
            local warnings=$(grep -c "warning:" "$REPORTDIR/compile_test_$basename.log" 2>/dev/null || echo "0")
            warning_count=$((warning_count + warnings))
            
            rm -f /tmp/test_compile.o
        fi
    done
    
    echo "Compilation Summary: $error_count errors, $warning_count warnings" > "$REPORTDIR/compilation_summary.txt"
    print_success "Compilation check complete"
}

# Run cppcheck static analysis
run_cppcheck() {
    print_status "Running cppcheck static analysis..."
    
    if check_tool cppcheck; then
        cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem \
                 --output-file="$REPORTDIR/cppcheck_report.txt" \
                 --xml --xml-version=2 \
                 "$SRCDIR" "$TESTDIR" "$INCDIR" 2>&1 | tee "$REPORTDIR/cppcheck_console.txt"
        
        # Parse results
        local error_count=$(grep -c "error" "$REPORTDIR/cppcheck_report.txt" 2>/dev/null || echo "0")
        local warning_count=$(grep -c "warning" "$REPORTDIR/cppcheck_report.txt" 2>/dev/null || echo "0")
        
        echo "Cppcheck Summary: $error_count errors, $warning_count warnings" >> "$REPORTDIR/quality_summary.txt"
        print_success "Cppcheck analysis complete"
    fi
}

# Run clang-tidy static analysis
run_clang_tidy() {
    print_status "Running clang-tidy static analysis..."
    
    if check_tool clang-tidy; then
        # Create compile_commands.json
        echo '[' > compile_commands.json
        local file_count=0
        for file in "$SRCDIR"/*.c; do
            if [[ -f "$file" ]]; then
                if [[ $file_count -gt 0 ]]; then
                    echo ',' >> compile_commands.json
                fi
                echo '  {' >> compile_commands.json
                echo '    "directory": "'$(pwd)'",' >> compile_commands.json
                echo '    "command": "'$CC' '$CFLAGS' -c '$file'",' >> compile_commands.json
                echo '    "file": "'$file'"' >> compile_commands.json
                echo '  }' >> compile_commands.json
                ((file_count++))
            fi
        done
        echo ']' >> compile_commands.json
        
        clang-tidy src/*.c -- -std=c11 -Wall -Wextra -O2 -Iinclude 2>&1 | tee "$REPORTDIR/clang_tidy_report.txt"
        
        # Parse results
        local error_count=$(grep -c "error:" "$REPORTDIR/clang_tidy_report.txt" 2>/dev/null || echo "0")
        local warning_count=$(grep -c "warning:" "$REPORTDIR/clang_tidy_report.txt" 2>/dev/null || echo "0")
        
        echo "Clang-tidy Summary: $error_count errors, $warning_count warnings" >> "$REPORTDIR/quality_summary.txt"
        print_success "Clang-tidy analysis complete"
    fi
}

# Run valgrind memory analysis
run_valgrind() {
    print_status "Running valgrind memory analysis..."
    
    if check_tool valgrind; then
        # Build a test executable
        if $CC $CFLAGS "$SRCDIR/01_types.c" -o /tmp/test_valgrind; then
            valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
                     --track-origins=yes --verbose --log-file="$REPORTDIR/valgrind_report.txt" \
                     /tmp/test_valgrind > /dev/null 2>&1
            
            # Parse results
            local leak_count=$(grep -c "definitely lost" "$REPORTDIR/valgrind_report.txt" 2>/dev/null || echo "0")
            local error_count=$(grep -c "ERROR SUMMARY" "$REPORTDIR/valgrind_report.txt" 2>/dev/null || echo "0")
            
            echo "Valgrind Summary: $leak_count memory leaks, $error_count errors" >> "$REPORTDIR/quality_summary.txt"
            print_success "Valgrind analysis complete"
            
            rm -f /tmp/test_valgrind
        else
            print_warning "Failed to build test executable for valgrind"
        fi
    fi
}

# Run AddressSanitizer
run_address_sanitizer() {
    print_status "Running AddressSanitizer..."
    
    # Build with AddressSanitizer
    if $CC -std=c11 -Wall -Wextra -O2 -Iinclude -fsanitize=address -fsanitize=undefined \
           -g "$SRCDIR/01_types.c" -o /tmp/test_asan 2>&1 | tee "$REPORTDIR/asan_build.log"; then
        
        # Run with AddressSanitizer
        ASAN_OPTIONS=detect_leaks=1:abort_on_error=0 /tmp/test_asan 2>&1 | tee "$REPORTDIR/asan_report.txt"
        
        # Parse results
        local error_count=$(grep -c "ERROR:" "$REPORTDIR/asan_report.txt" 2>/dev/null || echo "0")
        local leak_count=$(grep -c "Direct leak" "$REPORTDIR/asan_report.txt" 2>/dev/null || echo "0")
        
        echo "AddressSanitizer Summary: $error_count errors, $leak_count leaks" >> "$REPORTDIR/quality_summary.txt"
        print_success "AddressSanitizer analysis complete"
        
        rm -f /tmp/test_asan
    else
        print_warning "Failed to build with AddressSanitizer"
    fi
}

# Check code style
check_code_style() {
    print_status "Checking code style..."
    
    local style_issues=0
    
    # Check for tabs vs spaces
    local tab_count=$(find "$SRCDIR" "$TESTDIR" "$INCDIR" -name "*.c" -o -name "*.h" | xargs grep -c $'\t' 2>/dev/null | awk '{sum += $1} END {print sum+0}')
    if [[ $tab_count -gt 0 ]]; then
        echo "Style Issue: Found $tab_count tab characters (use spaces instead)" >> "$REPORTDIR/style_issues.txt"
        ((style_issues++))
    fi
    
    # Check for trailing whitespace
    local trailing_ws=$(find "$SRCDIR" "$TESTDIR" "$INCDIR" -name "*.c" -o -name "*.h" | xargs grep -c '[[:space:]]$' 2>/dev/null | awk '{sum += $1} END {print sum+0}')
    if [[ $trailing_ws -gt 0 ]]; then
        echo "Style Issue: Found $trailing_ws lines with trailing whitespace" >> "$REPORTDIR/style_issues.txt"
        ((style_issues++))
    fi
    
    # Check for long lines
    local long_lines=$(find "$SRCDIR" "$TESTDIR" "$INCDIR" -name "*.c" -o -name "*.h" | xargs awk 'length > 100 {count++} END {print count+0}')
    if [[ $long_lines -gt 0 ]]; then
        echo "Style Issue: Found $long_lines lines longer than 100 characters" >> "$REPORTDIR/style_issues.txt"
        ((style_issues++))
    fi
    
    # Check for missing newline at end of file
    local missing_newline=0
    for file in "$SRCDIR"/*.c "$TESTDIR"/*.c "$INCDIR"/*.h; do
        if [[ -f "$file" ]]; then
            if [[ -n "$(tail -c1 "$file")" ]]; then
                echo "Style Issue: $file missing newline at end of file" >> "$REPORTDIR/style_issues.txt"
                ((missing_newline++))
            fi
        fi
    done
    
    echo "Code Style Summary: $style_issues issues found" >> "$REPORTDIR/quality_summary.txt"
    print_success "Code style check complete"
}

# Check for common bugs
check_common_bugs() {
    print_status "Checking for common bugs..."
    
    local bug_count=0
    
    # Check for potential buffer overflows
    local buffer_overflows=$(grep -r "strcpy\|strcat\|sprintf" "$SRCDIR" "$TESTDIR" 2>/dev/null | wc -l)
    if [[ $buffer_overflows -gt 0 ]]; then
        echo "Potential Bug: Found $buffer_overflows uses of unsafe string functions" >> "$REPORTDIR/bug_issues.txt"
        ((bug_count++))
    fi
    
    # Check for uninitialized variables
    local uninit_vars=$(grep -r "int.*;" "$SRCDIR" "$TESTDIR" 2>/dev/null | grep -v "= " | wc -l)
    if [[ $uninit_vars -gt 0 ]]; then
        echo "Potential Bug: Found $uninit_vars potentially uninitialized variables" >> "$REPORTDIR/bug_issues.txt"
        ((bug_count++))
    fi
    
    # Check for memory leaks
    local malloc_count=$(grep -r "malloc\|calloc\|realloc" "$SRCDIR" "$TESTDIR" 2>/dev/null | wc -l)
    local free_count=$(grep -r "free" "$SRCDIR" "$TESTDIR" 2>/dev/null | wc -l)
    if [[ $malloc_count -ne $free_count ]]; then
        echo "Potential Bug: Mismatch between malloc ($malloc_count) and free ($free_count) calls" >> "$REPORTDIR/bug_issues.txt"
        ((bug_count++))
    fi
    
    # Check for null pointer dereferences
    local null_derefs=$(grep -r "->" "$SRCDIR" "$TESTDIR" 2>/dev/null | grep -v "if.*!=" | wc -l)
    if [[ $null_derefs -gt 0 ]]; then
        echo "Potential Bug: Found $null_derefs potential null pointer dereferences" >> "$REPORTDIR/bug_issues.txt"
        ((bug_count++))
    fi
    
    echo "Bug Check Summary: $bug_count potential issues found" >> "$REPORTDIR/quality_summary.txt"
    print_success "Bug check complete"
}

# Generate comprehensive report
generate_report() {
    print_status "Generating comprehensive quality report..."
    
    local report_file="$REPORTDIR/quality_report.html"
    
    cat > "$report_file" << 'EOF'
<!DOCTYPE html>
<html>
<head>
    <title>C Code Quality Report</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .header { background-color: #f0f0f0; padding: 20px; border-radius: 5px; }
        .section { margin: 20px 0; padding: 15px; border: 1px solid #ddd; border-radius: 5px; }
        .success { color: green; }
        .warning { color: orange; }
        .error { color: red; }
        .summary { background-color: #e8f4f8; padding: 15px; border-radius: 5px; }
        pre { background-color: #f5f5f5; padding: 10px; border-radius: 3px; overflow-x: auto; }
    </style>
</head>
<body>
    <div class="header">
        <h1>C Code Quality Report</h1>
        <p>Generated on: $(date)</p>
    </div>
EOF

    # Add summary section
    if [[ -f "$REPORTDIR/quality_summary.txt" ]]; then
        echo "    <div class='summary'>" >> "$report_file"
        echo "        <h2>Quality Summary</h2>" >> "$report_file"
        echo "        <pre>" >> "$report_file"
        cat "$REPORTDIR/quality_summary.txt" >> "$report_file"
        echo "        </pre>" >> "$report_file"
        echo "    </div>" >> "$report_file"
    fi
    
    # Add compilation results
    if [[ -f "$REPORTDIR/compilation_summary.txt" ]]; then
        echo "    <div class='section'>" >> "$report_file"
        echo "        <h2>Compilation Results</h2>" >> "$report_file"
        echo "        <pre>" >> "$report_file"
        cat "$REPORTDIR/compilation_summary.txt" >> "$report_file"
        echo "        </pre>" >> "$report_file"
        echo "    </div>" >> "$report_file"
    fi
    
    # Add style issues
    if [[ -f "$REPORTDIR/style_issues.txt" ]]; then
        echo "    <div class='section'>" >> "$report_file"
        echo "        <h2>Code Style Issues</h2>" >> "$report_file"
        echo "        <pre>" >> "$report_file"
        cat "$REPORTDIR/style_issues.txt" >> "$report_file"
        echo "        </pre>" >> "$report_file"
        echo "    </div>" >> "$report_file"
    fi
    
    # Add bug issues
    if [[ -f "$REPORTDIR/bug_issues.txt" ]]; then
        echo "    <div class='section'>" >> "$report_file"
        echo "        <h2>Potential Bugs</h2>" >> "$report_file"
        echo "        <pre>" >> "$report_file"
        cat "$REPORTDIR/bug_issues.txt" >> "$report_file"
        echo "        </pre>" >> "$report_file"
        echo "    </div>" >> "$report_file"
    fi
    
    echo "</body></html>" >> "$report_file"
    
    print_success "Quality report generated: $report_file"
}

# Clean up
cleanup() {
    print_status "Cleaning up temporary files..."
    rm -f /tmp/test_*.o /tmp/test_asan /tmp/test_valgrind
    rm -f compile_commands.json
    print_success "Cleanup complete"
}

# Show help
show_help() {
    echo "C Code Quality Checker Script"
    echo "============================="
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  compile     - Check compilation with strict warnings"
    echo "  cppcheck    - Run cppcheck static analysis"
    echo "  clang-tidy  - Run clang-tidy static analysis"
    echo "  valgrind    - Run valgrind memory analysis"
    echo "  asan        - Run AddressSanitizer"
    echo "  style       - Check code style"
    echo "  bugs        - Check for common bugs"
    echo "  report      - Generate comprehensive report"
    echo "  all         - Run all quality checks"
    echo "  clean       - Clean up temporary files"
    echo "  help        - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 compile    # Check compilation"
    echo "  $0 all        # Run all checks"
    echo "  $0 clean      # Clean up"
}

# Main function
main() {
    create_report_dir
    
    case "${1:-help}" in
        "compile")
            check_compilation
            ;;
        "cppcheck")
            run_cppcheck
            ;;
        "clang-tidy")
            run_clang_tidy
            ;;
        "valgrind")
            run_valgrind
            ;;
        "asan")
            run_address_sanitizer
            ;;
        "style")
            check_code_style
            ;;
        "bugs")
            check_common_bugs
            ;;
        "report")
            generate_report
            ;;
        "all")
            check_compilation
            run_cppcheck
            run_clang_tidy
            run_valgrind
            run_address_sanitizer
            check_code_style
            check_common_bugs
            generate_report
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
