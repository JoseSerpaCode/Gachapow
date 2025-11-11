# run.ps1 - Compila todo el proyecto y ejecuta el juego (corregido)
Write-Host "🔧 Preparando compilación..."

$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$srcDir = Join-Path $PSScriptRoot "src"

# Recolecta .c recursivamente
$files = Get-ChildItem -Path $srcDir -Recurse -Filter *.c | ForEach-Object { $_.FullName }

if (-not $files) {
    Write-Error "No se encontraron archivos .c en $srcDir"
    exit 1
}

# Asegurar build dir
$buildDir = Join-Path $PSScriptRoot "build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }

# Includes y libs
$includes = @(
    "-I", (Join-Path $PSScriptRoot "include"),
    "-I", (Join-Path $PSScriptRoot "include\core"),
    "-I", (Join-Path $PSScriptRoot "include\extern"),
    "-I", (Join-Path $PSScriptRoot "include\gameplay"),
    "-I", (Join-Path $PSScriptRoot "include\states"),
    "-I", (Join-Path $PSScriptRoot "include\hw")
)

$libs = @("-L", (Join-Path $PSScriptRoot "lib"), "-lraylib", "-lgdi32", "-lwinmm")
$otherFlags = @("-Wall", "-Wextra", "-g")

$outExe = Join-Path $buildDir "game.exe"
$logFile = Join-Path $buildDir "build.log"

# Construir lista de argumentos (flatten)
$args = @()
$args += $files
$args += $includes
$args += $libs
$args += $otherFlags
$args += "-o"
$args += $outExe

# Mostrar preview
Write-Host "Archivos a compilar: $($files.Count)"
Write-Host "Salida: $outExe"
Write-Host "Guardando log en: $logFile"
Write-Host "Ejecutando gcc..."
Write-Host ">> gcc " + ($args -join ' ')

# Ejecutar gcc y capturar output (stdout+stderr), volcar a log y a consola
# Usamos call operator & y redirección 2>&1, luego Tee-Object para log
$gccPath = "gcc"
$cmd = "& `"$gccPath`" " + ($args | ForEach-Object { if ($_.Contains(' ')) { '"{0}"' -f $_ } else { $_ } } ) + " 2>&1"
# Ejecutar comando en un subshell para respetar redirección
$procOutput = powershell -NoProfile -Command $cmd | Tee-Object -FilePath $logFile

# Obtener exitcode de la última ejecución de gcc
# Cuando invocamos powershell -Command, $LASTEXITCODE no se propaga automáticamente.
# Entonces leemos la última línea del log buscando "gcc" exit - mejor: ejecutar gcc directamente y usar $LASTEXITCODE.
# Para mayor fiabilidad, ejecutemos gcc directamente y luego redirigimos su salida:
& $gccPath @args 2>&1 | Tee-Object -FilePath $logFile
$exit = $LASTEXITCODE

if ($exit -eq 0) {
    Write-Host "✅ Compilación exitosa. Ejecutando el juego..."
    & $outExe
    exit 0
} else {
    Write-Host "❌ Error de compilación. Revisa $logFile"
    Write-Host "`nÚltimas 40 líneas de log:`n"
    Get-Content $logFile -Tail 40 | ForEach-Object { Write-Host $_ }
    exit $exit
}
