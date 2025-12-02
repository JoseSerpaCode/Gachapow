param(
    [switch]$pc,
    [switch]$embedded,
    [switch]$clean
)

Write-Host "🔧 Gachapow Build System" -ForegroundColor Cyan

$root = Split-Path -Parent $MyInvocation.MyCommand.Definition
$srcDir = Join-Path $root "src"
$buildDir = Join-Path $root "build"
$includeDir = Join-Path $root "include"
$logFile = Join-Path $buildDir "build.log"
$outExe = Join-Path $buildDir "game.exe"

# -----------------------------------------
# CLEAN
# -----------------------------------------
if ($clean) {
    if (Test-Path $buildDir) {
        Remove-Item -Recurse -Force $buildDir
        Write-Host "🧹 Build limpiado."
    } else {
        Write-Host "No hay build/ para limpiar."
    }
    exit 0
}

# -----------------------------------------
# SELECCIÓN DE PLATAFORMA
# -----------------------------------------
if ($pc -and $embedded) {
    Write-Host "❌ ERROR: No puedes elegir PC y EMBEDDED simultáneamente." -ForegroundColor Red
    exit 1
}

$platform = ""
if ($pc)       { $platform = "PC" }
elseif ($embedded) { $platform = "EMBEDDED" }
else {
    Write-Host "⚠️ No se eligió plataforma. Usando PC por defecto."
    $platform = "PC"
}

Write-Host "🔎 Plataforma seleccionada: $platform" -ForegroundColor Yellow

# -----------------------------------------
# LISTA DE ARCHIVOS A COMPILAR
# -----------------------------------------
$files = Get-ChildItem -Path $srcDir -Recurse -Filter *.c

switch ($platform) {
    "PC" {
        $files = $files | Where-Object { $_.Name -notmatch "hw_embedded.c" }
        $defines = @("-DHW_PC")
    }
    "EMBEDDED" {
        $files = $files | Where-Object { $_.Name -notmatch "hw_pc.c" }
        $defines = @("-DHW_EMBEDDED")
    }
}

$files = $files | ForEach-Object { $_.FullName }

# -----------------------------------------
# Preparar build/
# -----------------------------------------
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# -----------------------------------------
# FLAGS / LIBS
# -----------------------------------------
$includes = @(
    "-I", $includeDir,
    "-I", "$includeDir/core",
    "-I", "$includeDir/extern",
    "-I", "$includeDir/gameplay",
    "-I", "$includeDir/states",
    "-I", "$includeDir/hw"
)

$libs = @(
    "-L", (Join-Path $root "lib"),
    "-lraylib",
    "-lgdi32",
    "-lwinmm"
)

$otherFlags = @("-Wall", "-Wextra", "-g")

# -----------------------------------------
# ARMAR ARGUMENTOS GCC
# -----------------------------------------
$args = @()
$args += $files
$args += $includes
$args += $libs
$args += $defines
$args += $otherFlags
$args += "-o"
$args += $outExe

Write-Host "📦 Compilando $($files.Count) archivos..."
Write-Host ">> gcc " + ($args -join " ")

# -----------------------------------------
# EJECUTAR COMPILACIÓN
# -----------------------------------------
& gcc @args 2>&1 | Tee-Object -FilePath $logFile
$exit = $LASTEXITCODE

if ($exit -eq 0) {
    Write-Host "✅ Build exitoso. Ejecutando..." -ForegroundColor Green
    & $outExe
    exit 0
} else {
    Write-Host "❌ Error de compilación." -ForegroundColor Red
    Write-Host "📄 Últimas 40 líneas del log:"
    Get-Content $logFile -Tail 40
    exit $exit
}
