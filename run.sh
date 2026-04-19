#!/bin/bash

# 🔧 Gachapow Build System (Linux)

PLATFORM=""
CLEAN=false

for arg in "$@"; do
    case $arg in
        --pc)       PLATFORM="PC" ;;
        --embedded) PLATFORM="EMBEDDED" ;;
        --clean)    CLEAN=true ;;
    esac
done

ROOT="$(cd "$(dirname "$0")" && pwd)"
SRC="$ROOT/src"
BUILD="$ROOT/build"
INCLUDE="$ROOT/include"
LOG="$BUILD/build.log"
OUT="$BUILD/game"

echo "🔧 Gachapow Build System"

# -----------------------------------------
# CLEAN
# -----------------------------------------
if $CLEAN; then
    if [ -d "$BUILD" ]; then
        rm -rf "$BUILD"
        echo "🧹 Build limpiado."
    else
        echo "No hay build/ para limpiar."
    fi
    exit 0
fi

# -----------------------------------------
# SELECCIÓN DE PLATAFORMA
# -----------------------------------------
if [ "$1" = "--pc" ] && [ "$2" = "--embedded" ] || \
   [ "$1" = "--embedded" ] && [ "$2" = "--pc" ]; then
    echo "❌ ERROR: No puedes elegir PC y EMBEDDED simultáneamente."
    exit 1
fi

if [ -z "$PLATFORM" ]; then
    echo "⚠️  No se eligió plataforma. Usando PC por defecto."
    PLATFORM="PC"
fi

echo "🔎 Plataforma seleccionada: $PLATFORM"

# -----------------------------------------
# LISTA DE ARCHIVOS A COMPILAR
# -----------------------------------------
if [ "$PLATFORM" = "PC" ]; then
    FILES=$(find "$SRC" -name "*.c" | grep -v "hw_embedded.c")
    DEFINES="-DHW_PC"
elif [ "$PLATFORM" = "EMBEDDED" ]; then
    FILES=$(find "$SRC" -name "*.c" | grep -v "hw_pc.c")
    DEFINES="-DHW_EMBEDDED"
fi

FILE_COUNT=$(echo "$FILES" | wc -l)

# -----------------------------------------
# PREPARAR build/
# -----------------------------------------
mkdir -p "$BUILD"

# -----------------------------------------
# FLAGS / LIBS
# -----------------------------------------
INCLUDES=(
    -I "$INCLUDE"
    -I "$INCLUDE/core"
    -I "$INCLUDE/extern"
    -I "$INCLUDE/gameplay"
    -I "$INCLUDE/states"
    -I "$INCLUDE/hw"
)

# En Linux raylib usa estas libs en lugar de gdi32/winmm
LIBS=(
    -L "$ROOT/lib"
    -lraylib
    -lGL
    -lm
    -lpthread
    -ldl
    -lrt
    -lX11
)

OTHER_FLAGS=(-Wall -Wextra -g)

# -----------------------------------------
# COMPILAR
# -----------------------------------------
echo "📦 Compilando $FILE_COUNT archivos..."
echo ">> gcc $FILES ${INCLUDES[*]} ${LIBS[*]} $DEFINES ${OTHER_FLAGS[*]} -o $OUT"

gcc $FILES \
    "${INCLUDES[@]}" \
    "${LIBS[@]}" \
    $DEFINES \
    "${OTHER_FLAGS[@]}" \
    -o "$OUT" 2>&1 | tee "$LOG"

EXIT_CODE=${PIPESTATUS[0]}

# -----------------------------------------
# RESULTADO
# -----------------------------------------
if [ $EXIT_CODE -eq 0 ]; then
    echo "✅ Build exitoso. Ejecutando..."
    "$OUT"
else
    echo "❌ Error de compilación."
    echo "📄 Últimas 40 líneas del log:"
    tail -n 40 "$LOG"
    exit $EXIT_CODE
fi
