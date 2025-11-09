# Compila todo el proyecto con Raylib y ejecuta el juego
Write-Host "🔧 Compilando el juego..."

# Buscar todos los archivos .c en src/ (recursivamente)
$files = Get-ChildItem -Path "src" -Recurse -Filter *.c | ForEach-Object { $_.FullName }

# Compilación con includes correctos
gcc $files `
    -I include `
    -I include/core `
    -I include/extern `
    -I include/gameplay `
    -I include/states `
    -I include/hw `
    -L lib -lraylib -lgdi32 -lwinmm -Wall -Wextra -o build/game.exe

# Verificar si la compilación fue exitosa
if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Compilación exitosa. Ejecutando el juego..."
    ./build/game.exe
} else {
    Write-Host "❌ Error de compilación."
}
