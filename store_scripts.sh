#!/bin/bash

scripts_dir=~/Scripts/android
cp set_android_environment.sh "$scripts_dir"

p="$scripts_dir/SDL"

cp android_build.sh "$p"
cp android_build_and_run.sh "$p"
cp android_config.sh "$p"
cp android_init.sh "$p"
cp android_init_and_build.sh "$p"
cp android_init_build_run.sh "$p"
cp android_run.sh "$p"
cp build.gradle.replace "$p"
cp jni_CMakeLists.txt.replace "$p"
