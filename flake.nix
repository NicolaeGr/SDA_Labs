{
  description = "Lab1 - C Sorting Algorithms Project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            cmake
            gnumake
            ninja

            clang
            clang-tools
            llvmPackages.libclang
            gcc

            gdb
            valgrind

            clang-tools
            shfmt
            cppcheck

            pkg-config
          ];

          CMAKE_EXPORT_COMPILE_COMMANDS = "ON";
          CC = "clang";
          CXX = "clang++";
        };
      }
    );
}
