
function job()
{
	local THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	source $THIS_DIR/os.sh

	if is_windows; then
		export arkanoid_deps="${HOME}/Projects"
	else
		export arkanoid_deps="${HOME}/Projects"
	fi

	[ ! -z arkanoid_deps ] && build_deps=$arkanoid_deps
}

job $@