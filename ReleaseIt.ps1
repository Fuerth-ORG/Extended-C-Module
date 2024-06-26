################################################################################
# Configs

$libVersion = "1.0";
# Die Dateien, die für das Include-Release benötigt werden
$incExtensions = @(
    ".h",
    ".hpp",
	".inl"
)
$srcExtensions = @(
    ".c",
    ".cpp"
)
#Konfiguration für Windows Release-Extensions
$windowsReleaseExtensions = @(
    ".dll",
    ".lib"
)
# Konfiguration für Platformunabhängige Releaseordner
$releases = @(
    @{
        "name"              = "includes";
        "zipFile"           = ".\RELEASE\ecm-$libVersion-includes.7z";
        "src_dir"           = "";
        "src_extensions"    = @();
        "dirs" = @(
            ".\RELEASE\include\"
        )
    }, @{
        "name"              = "win32_x64";
        "zipfile"           = ".\RELEASE\ecm-$libVersion-win32-x64.7z";
        "src_dir"           = "Release-x64\";
        "src_extensions"    = $windowsReleaseExtensions;
        "dirs" = @(
            ".\RELEASE\Release-x64\"
        )
    }, @{
        "name"              = "win32_x86";
        "zipfile"           = ".\RELEASE\ecm-$libVersion-win32-x86.7z";
        "src_dir"           = "Release-Win32\";
        "src_extensions"    = $windowsReleaseExtensions;
        "dirs" = @(
            ".\RELEASE\Release-Win32\"
        )
    }, @{
        "name"              = "win32_x64_dev";
        "zipfile"           = ".\RELEASE\ecm-$libVersion-dev-win32-x64.7z";
        "src_dir"           = "Release-x64\";
        "src_extensions"    = $windowsReleaseExtensions;
        "dirs" = @(
            ".\RELEASE\Release-x64\",
            ".\RELEASE\include\"
        )
    }, @{
        "name"              = "win32_x86_dev";
        "zipfile"           = ".\RELEASE\ecm-$libVersion-dev-win32-x86.7z";
        "src_dir"           = "Release-Win32\";
        "src_extensions"    = $windowsReleaseExtensions;
        "dirs" = @(
            ".\RELEASE\Release-Win32\",
            ".\RELEASE\include\"
        )
    }
)

################################################################################
# Static data

$licenseText = [IO.File]::ReadAllText("LICENSE");

################################################################################
# Functions

function CopyAllFileExtensions {
    param (
        [string]$path,
        [string]$targetPath,
        [string[]]$extensions,
        [bool]$copyrightSign = 0
    )
    # Überprüft , ob das Zielpfadverzeichnis existiert, andernfalls erstellen
    if (-not (Test-Path $targetPath)) {
        New-Item -ItemType Directory -Path $targetPath -Force
    }
    # Konvertiere den Eingabepfad in einen absoluten Pfad
    $path = Resolve-Path -Path $path
    # Hole alle Dateien mit den angegebenen Extensions rekursiv
    Get-ChildItem -Path $path -Recurse | Where-Object {
        $extensions -contains $_.Extension -and !$_.PSIsContainer
    } | ForEach-Object {
        $relativePath = $_.FullName.Substring($path.Length)
        $destinationPath = Join-Path -Path $targetPath -ChildPath $relativePath
        $destinationDir = Split-Path -Path $destinationPath -Parent
        # Überprüfen, ob das Zielverzeichnis existiert, andernfalls erstellen
        if (-not (Test-Path $destinationDir)) {
            New-Item -ItemType Directory -Path $destinationDir -Force
        }
        Write-Host "Kopiere $relativePath nach $destinationPath"

        # Prüfe ob Copyright-Text hinzugefügt werden soll
        if ($copyrightSign -eq 1) {
            # Füge Copyright-Text hinzu
            $srcFileContent = [IO.File]::ReadAllText($_.FullName).Trim();
            $outContent = "/*`n" + $licenseText + "*/`n`n" + $srcFileContent;
            # Kopiere Datei
            Set-Content -Path $destinationPath -Value $outContent -Force;
        } else {
            # Kopiert eine Datei ins Ziel(unter)verzeichnis
            Copy-Item -Path $_.FullName -Destination $destinationPath -Force
        }
    }
}

function AddFileToZip {
    param (
        [string]$zipFile,
        [string]$file
    )
    &"C:\Program Files\7-Zip\7z.exe" a -r -mx9 $zipFile $file
}

function AddFullDirectoryToZip {
    param (
        [string]$zipFile,
        [string]$dir
    )
    &"C:\Program Files\7-Zip\7z.exe" a -r -mx9 $zipFile $dir
}

################################################################################
# Application

# Kopiere die Include-Dateien
Write-Host "Copy include files..."
CopyAllFileExtensions -path "libs\ecm" -targetPath "RELEASE\include\ecm" -extensions $incExtensions -copyrightSign 1
# Kopiere die Source-Dateien
Write-Host "Copy source files..."
CopyAllFileExtensions -path "libs\source" -targetPath "RELEASE\source" -extensions $srcExtensions -copyrightSign 1
# Kopiere die Binaries und Packe die Release-Zips
foreach ($release in $releases) {
    $srcDir = $release.src_dir
    $zipFile = $release.zipfile
    Remove-Item -Path $zipfile
    CopyAllFileExtensions -path $srcDir -targetPath "RELEASE\$srcDir" -extensions $release.src_extensions
    foreach ($dir in $release.dirs) {
        AddFullDirectoryToZip -zipFile $zipFile -dir $dir
    }
    AddFileToZip -zipFile $zipFile -file "LICENSE"
}

Write-Host "`nRelease-Tool vollständig durchgelaufen"
Start-Sleep -Seconds (60 * 1)


