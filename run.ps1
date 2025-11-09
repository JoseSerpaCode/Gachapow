# Compila todo el proyecto con Raylib y ejecuta el juego
Write-Host "🔧 Compilando el juego..."

# Buscar todos los archivos .c en src/
$files = Get-ChildItem -Path "src" -Recurse -Filter *.c | ForEach-Object { $_.FullName }

# Compilación
gcc $files -I include -I include/extern -L lib -lraylib -lgdi32 -lwinmm -Wall -Wextra -o build/game.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Compilación exitosa. Ejecutando el juego..."
    ./build/game.exe
} else {
    Write-Host "❌ Error de compilación."
}
