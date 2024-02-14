$cscPath = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\Roslyn\csc.exe"

$baseDir = "D:\Repos\GameEngineTDA572\Engine\src\Multiplayer"
$server = Join-Path $baseDir "Server\GameServer.cs"
#$client = Join-Path $baseDir "GameClient.cs"
$helpers = Join-Path $baseDir "Helpers.cs"

$serverExe = Join-Path $baseDir "Build\bin\GameServer.exe"
#$clientExe = Join-Path $baseDir "GameClient.exe"

# Clean
if (Test-Path $serverExe) {
    Remove-Item -Path $serverExe
}


#if (Test-Path $clientExe) {
#    Remove-Item -Path $clientExe
#}

# Compile
$serverArgs = "-out:$serverExe $server $helpers"
#$clientArgs = "-out:$clientExe $client $helpers"

Start-Process -FilePath $cscPath -ArgumentList $serverArgs -Wait
#Start-Process -FilePath $cscPath -ArgumentList $clientArgs -Wait


if (-not (Test-Path $serverExe)) {
    Write-Error "Failed to compile server. Try to compile it manually with:"
    Write-Host "csc $serverArgs"
    exit
}


#if (-not (Test-Path $clientExe)) {
#    Write-Error "Failed to compile client. Try to compile it manually with:"
#    Write-Host "csc $clientArgs"
#    exit
#}

Start-Process powershell {.\RunServer.ps1}
#Start-Process powershell {.\RunClient.ps1}