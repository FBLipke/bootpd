#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || exit 1

case "${1:-build}" in
    clean)
        rm -rf build
        find . -name "*.o" -delete
        echo "Cleaned"
        ;;
    *)
        rm -rf build
        cmake -S . -B build
        
        # Kein Argument = alles bauen
        if [ -z "$1" ]; then
            cmake --build build
        else
            # Plugin specified?
            cmake --build build --target "$1"
        fi
        
        rm -rf build
        echo "Build complete: bin/"
        ;;
esac
