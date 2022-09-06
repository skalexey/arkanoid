
THIS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source $THIS_DIR/os.sh

if is_windows; then
	export arkanoid_deps="${HOME}/Projects"
	export build_libs="${HOME}/libs"
else
	export Arkanoid_deps="${HOME}/Projects"
	export build_libs="${HOME}/libs"
fi