#!/bin/bash

# This script follows instructions given in the README-android.md by the link:
# https://github.com/libsdl-org/SDL/blob/main/docs/README-android.md

job()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	cd "$THIS_DIR"

	source file_utils.sh

	# Use library instead of executable for the android app

	# Load common variables
	source android_config.sh

	# Setup logger
	local folderName=${PWD##*/}

	source log.sh
	local log_prefix="-- [${folderName} android build script]: "

	# Set project's environment variables
	source external_config.sh

	# Uninstall the app first
	# adb uninstall "$app"

	# Store the current directory and go to the android project 
	local root_dir="${PWD}"
	cd $proj

	# Run android build
	./gradlew installDebug

	# Return executable back
	cd "$root_dir"
	#file_replace "CMakeLists.txt" "add_library" "add_executable"
	
	log_success "Finished build" " -" " ---"
}

job $@