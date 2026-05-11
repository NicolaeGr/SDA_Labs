setup:
    cmake -B build -S .

run project:
    cmake --build build --target {{project}}
    ./build/{{project}}/{{project}}

list:
    cmake --build build --target help