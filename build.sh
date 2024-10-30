#!/bin/bash

build_dir="build"
install_dir="/usr/local/bin"
filename="todo_list"
install=false

if [ "$1" = "--install" ]; then
	install=true
fi

if [ ! -d "$build_dir" ]; then
	mkdir "$build_dir"
fi

cd "$build_dir" || { echo "$build_dir directory doesn't exist"; exit 1; }

cmake .. || { echo "CMake failed"; exit 1; }

make || { echo "Makefile failed"; exit 1; }

if [ "$install" = true ]; then
	sudo mv "$filename" "$install_dir" || { echo "Moving $filename to $install_dir failed"; exit 1; }
else
	mv "$filename" .. || { echo "Moving $filename failed"; exit 1; }
fi

echo "Build completed successfully!"

exit 0;
