# do to the library dependency that was not modeled correctly in CMake,
# I need to manually specify the linking order.
# This is a temporary solution until the CMakeLists.txt is updated.

$projectPath = Get-Location
$projectPath = $projectPath.ToString()

# enumerate all libraries
$libraries = @("std",
    "ancillarycat.api.ansi",
    "ancillarycat.api.windows",
    "ancillarycat.api.generator",
    "ancillarycat.api",
    "ancillarycat.console.console",
    "ancillarycat.console.point",
    "ancillarycat.console",
    "ancillarycat.entities.entity",
    "ancillarycat.entities.snake",
    "ancillarycat.entities.food",
    "ancillarycat.entities",
    "ancillarycat.game.game",
    "ancillarycat.game.leaderboard",
    "ancillarycat.game.config",
    "ancillarycat.game",
    "ancillarycat.handler",
    "ancillarycat.blocks.block",
    "ancillarycat.blocks.obstacle",
    "ancillarycat.blocks"
)

# usage: script.ps1 <projectName> <buildDir> or script.ps1 <buildDir>
# if no argument is provided, the default is "build"
$buildDirName = "build"
$projectName = "snakeRebuild"
#if ($args.Length -eq 1)
#{
#    $buildDirName = $args[0]
#}
#if ($args.Length -eq 2)
#{
#    $projectName = $args[0]
#    $buildDirName = $args[1]
#}

$buildDir = $projectPath + "\" + $buildDirName
# check if the build directory exists,m if exists, prompt the user to delete it, or exit
#if (Test-Path $buildDirName)
#{
#    Write-Host "The build directory already exists. Do you want to delete it? (y/n)"
#    $response = Read-Host
#    if ($response -eq "y")
#    {
#        Remove-Item -Recurse -Force $buildDir
#
#        # create the build directory
#        New-Item -ItemType Directory -Path $buildDirName | Out-Null
#    }
#}
#
## run cmake
#cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -DCMAKE_SYSTEM_NAME=WindowsStore -S $projectPath -B $buildDir

# run ninja one by one, specify the linking order
foreach ($library in $libraries)
{
    Write-Host "Building $library..."
    # if build succeeds, continue to the next library, otherwise, exit
    cmake --build $buildDir --target $library -j 30
    if ($LASTEXITCODE -ne 0)
    {
        Write-Host "Failed to build $library."
        #        Start-Sleep -s 1
        #        exit
    }
    else
    {
        # sleep for 1 second
        #        Start-Sleep -s 1
        Write-Host "Done building $library."
    }
}

Start-Sleep -s 1

cmake --build $buildDir --target $projectName -j 30

if ($LASTEXITCODE -ne 0)
{
    Write-Host "Failed to build $projectName."
    # pause for 3 seconds
    Start-Sleep -s 3
    Write-Host "Abort."
    #    exit
}
else
{
    Write-Host "Done building $projectName."
    Start-Sleep -s 1
    Write-Host "Launching $projectName..."
    Start-Process $buildDir\$projectName.exe
}


