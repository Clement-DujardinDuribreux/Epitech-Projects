#!/bin/bash
# ============================================================
# install_sfml.sh — Install SFML 3.0.2 on Linux (from source)
# ============================================================
set -euo pipefail

SFML_VERSION="3.0.2"
SFML_URL="https://github.com/SFML/SFML/archive/refs/tags/${SFML_VERSION}.tar.gz"
BUILD_DIR="/tmp/sfml-build"
INSTALL_PREFIX="/usr/local"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

info()    { echo -e "${CYAN}[INFO]${NC}  $*"; }
success() { echo -e "${GREEN}[OK]${NC}    $*"; }
warn()    { echo -e "${YELLOW}[WARN]${NC}  $*"; }
error()   { echo -e "${RED}[ERROR]${NC} $*" >&2; exit 1; }

# ── Detect distro ─────────────────────────────────────────
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    else
        error "Cannot detect Linux distribution."
    fi
}

# ── Install system dependencies ───────────────────────────
install_deps() {
    local distro
    distro=$(detect_distro)
    info "Detected distro: $distro"

    case "$distro" in
        ubuntu|debian|linuxmint|pop)
            info "Installing dependencies via apt..."
            apt-get update -qq
            apt-get install -y --no-install-recommends \
                build-essential cmake git curl tar \
                libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
                libgl-dev libglx-dev libudev-dev \
                libfreetype-dev libopenal-dev \
                libflac-dev libvorbis-dev libogg-dev \
                libpthread-stubs0-dev libxinerama-dev \
                libxrender-dev libxkbcommon-dev \
                2>/dev/null
            ;;
        fedora|rhel|centos|almalinux|rocky)
            info "Installing dependencies via dnf/yum..."
            local PM="dnf"
            command -v dnf &>/dev/null || PM="yum"
            $PM install -y \
                gcc gcc-c++ cmake git curl tar \
                libX11-devel libXrandr-devel libXcursor-devel libXi-devel \
                mesa-libGL-devel mesa-libGLU-devel \
                systemd-devel freetype-devel openal-soft-devel \
                flac-devel libvorbis-devel libogg-devel \
                libXinerama-devel libXrender-devel \
                2>/dev/null
            ;;
        arch|manjaro|endeavouros)
            info "Installing dependencies via pacman..."
            pacman -Sy --noconfirm --needed \
                base-devel cmake git curl \
                libx11 libxrandr libxcursor libxi \
                mesa libudev0 freetype2 openal \
                libflac libvorbis libogg \
                libxinerama libxrender \
                2>/dev/null
            ;;
        opensuse*|sles)
            info "Installing dependencies via zypper..."
            zypper install -y \
                gcc gcc-c++ cmake git curl tar \
                libX11-devel libXrandr-devel libXcursor-devel libXi-devel \
                Mesa-libGL-devel systemd-devel \
                freetype2-devel openal-soft-devel \
                libFLAC-devel libvorbis-devel libogg-devel \
                2>/dev/null
            ;;
        alpine)
            info "Installing dependencies via apk..."
            apk add --no-cache \
                build-base cmake git curl tar \
                libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
                mesa-dev eudev-dev \
                freetype-dev openal-soft-dev \
                flac-dev libvorbis-dev libogg-dev \
                2>/dev/null
            ;;
        *)
            warn "Unknown distro '$distro'. Attempting Debian-style install..."
            apt-get update -qq && apt-get install -y --no-install-recommends \
                build-essential cmake git curl tar \
                libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
                libgl-dev libudev-dev libfreetype-dev libopenal-dev \
                libflac-dev libvorbis-dev libogg-dev \
                2>/dev/null || warn "Could not install all dependencies."
            ;;
    esac
    success "System dependencies installed."
}

# ── Check if SFML is already installed (shared libs) ──────
check_existing() {
    local has_cmake=false
    local has_shared=false
    local has_static=false

    [ -f "${INSTALL_PREFIX}/lib/cmake/SFML/SFMLConfig.cmake" ] && has_cmake=true
    ls "${INSTALL_PREFIX}/lib/libsfml-graphics.so"* 2>/dev/null | grep -q "." && has_shared=true
    ls "${INSTALL_PREFIX}/lib/libsfml-graphics.a"* 2>/dev/null | grep -q "."  && has_static=true

    if $has_shared; then
        warn "SFML 3.0.x shared libs (.so) already present at ${INSTALL_PREFIX}/lib. Skipping."
        exit 0
    fi

    if $has_static && ! $has_shared; then
        warn "Only static SFML libs (.a) found — will reinstall as shared (.so)."
        uninstall_static
        return
    fi

    if $has_cmake && ! $has_shared; then
        warn "SFML CMake config found but no .so libs — reinstalling."
        return
    fi
}

# ── Remove leftover static SFML installation ──────────────
uninstall_static() {
    info "Removing existing static SFML libs from ${INSTALL_PREFIX}..."
    rm -f  "${INSTALL_PREFIX}/lib/libsfml-"*.a
    rm -f  "${INSTALL_PREFIX}/lib/libsfml-"*.so*
    rm -rf "${INSTALL_PREFIX}/lib/cmake/SFML"
    rm -rf "${INSTALL_PREFIX}/include/SFML"
    rm -f  "${INSTALL_PREFIX}/lib/pkgconfig/sfml"*.pc
    success "Old SFML installation removed."
}

# ── Download & extract ────────────────────────────────────
download_sfml() {
    info "Downloading SFML ${SFML_VERSION}..."
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
    curl -fsSL "$SFML_URL" -o "${BUILD_DIR}/sfml.tar.gz"
    tar -xzf "${BUILD_DIR}/sfml.tar.gz" -C "$BUILD_DIR" --strip-components=1
    success "Downloaded and extracted SFML ${SFML_VERSION}."
}

# ── Build & install ───────────────────────────────────────
build_sfml() {
    info "Configuring SFML with CMake..."
    cmake -S "$BUILD_DIR" \
          -B "${BUILD_DIR}/build" \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
          -DSFML_BUILD_EXAMPLES=OFF \
          -DSFML_BUILD_DOC=OFF \
          -DSFML_BUILD_TEST_SUITE=OFF \
          -DCMAKE_INSTALL_RPATH="${INSTALL_PREFIX}/lib" \
          -DSFML_BUILD_AUDIO=ON \
          -DSFML_BUILD_GRAPHICS=ON \
          -DSFML_BUILD_WINDOW=ON \
          -DSFML_BUILD_NETWORK=ON \
          -DBUILD_SHARED_LIBS=ON \

    local NPROC
    NPROC=$(nproc 2>/dev/null || echo 4)
    info "Building SFML with ${NPROC} jobs..."
    cmake --build "${BUILD_DIR}/build" --parallel "$NPROC"

    info "Installing SFML to ${INSTALL_PREFIX}..."
    cmake --install "${BUILD_DIR}/build"
    success "SFML ${SFML_VERSION} installed to ${INSTALL_PREFIX}."
}

# ── Update linker cache ───────────────────────────────────
update_ldconfig() {
    if command -v ldconfig &>/dev/null; then
        ldconfig
        success "ldconfig updated."
    fi
}

# ── Clean up ──────────────────────────────────────────────
cleanup() {
    info "Cleaning up build directory..."
    rm -rf "$BUILD_DIR"
    success "Done."
}

# ── Verify installation ───────────────────────────────────
verify() {
    if [ -f "${INSTALL_PREFIX}/lib/cmake/SFML/SFMLConfig.cmake" ]; then
        success "SFML ${SFML_VERSION} is ready!"
        info "CMake config: ${INSTALL_PREFIX}/lib/cmake/SFML/SFMLConfig.cmake"
        info "Headers:      ${INSTALL_PREFIX}/include/SFML/"
        info "Libraries:    ${INSTALL_PREFIX}/lib/libsfml-*.so"
        echo ""
        echo -e "${CYAN}Usage in CMakeLists.txt:${NC}"
        echo '  find_package(SFML 3 COMPONENTS Graphics Window Network Audio System REQUIRED)'
        echo '  target_link_libraries(your_target PRIVATE SFML::Graphics SFML::Window SFML::Network SFML::Audio SFML::System)'
    else
        error "Installation verification failed. Check the output above."
    fi
}

# ── Main ──────────────────────────────────────────────────
main() {
    echo ""
    echo -e "${CYAN}╔══════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║   SFML ${SFML_VERSION} — Linux Installer          ║${NC}"
    echo -e "${CYAN}╚══════════════════════════════════════════╝${NC}"
    echo ""

    if [ "$EUID" -ne 0 ]; then
        error "This script must be run as root (use: sudo ./install_sfml.sh)"
    fi

    check_existing
    install_deps
    download_sfml
    build_sfml
    update_ldconfig
    cleanup
    verify
}

main "$@"
