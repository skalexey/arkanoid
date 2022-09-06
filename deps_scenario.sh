#!/bin/bash

function deps_scenario()
{
    source dependencies.sh
    source deps_config.sh

    download_dependency "SDL" "$depsLocation" "https://github.com/libsdl-org/SDL.git"
}

deps_scenario $@
