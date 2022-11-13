#!/bin/bash

job()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	cd "$THIS_DIR"

	local folderName=${PWD##*/}

	source log.sh
	local log_prefix="-- [${folderName} android init build run script]: "

	source android_init.sh
	source android_build.sh
	source android_run.sh

	log_success "Finished everything" " -" " ---"
}

job $@