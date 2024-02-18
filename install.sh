#!/bin/sh

printf "Discord 0.0.25\n"
printf "Discord desktop client based on Discord Web for FreeBSD\n"
printf "\nDo not run this in root! Or else this script breaks and will replicate your current chdir! It will not install under root!\n"

check_rsync() {
    if command -v rsync >/dev/null 2>&1; then
        return 0
    else
        return 1
    fi
}

install_rsync() {
    printf "rsync is not installed. Do you want to install it? (y/n): "
    read -r response
    if [ "$response" = "y" ]; then
        sudo pkg install -y rsync
    else
        printf "rsync is required for this script. Please install it manually and run the script again.\n"
        exit 1
    fi
}

install() {
    check_rsync || install_rsync

    printf "Installing...\n"
    
    SCRIPT_DIR="$(dirname "$0")"
    SCRIPTS_DIR="$SCRIPT_DIR/Scripts"
    cd "$SCRIPTS_DIR"

    g++ -std=c++20 MainScript.cpp SharedFunctions.cpp -o DownloadHandler -lcurl
    INSTALL_DIR="$HOME/.local"

    mkdir -p "$INSTALL_DIR/share/discord-bsd"
    mkdir -p "$INSTALL_DIR/bin"
    mkdir -p "$INSTALL_DIR/share/applications"

    # Copy using rsync with exclusion
    rsync -av --exclude='SharedFunctions.h' --exclude='SharedFunctions.cpp' --exclude='MainScript.cpp' "$SCRIPT_DIR/" "$INSTALL_DIR/share/discord-bsd/"

    mv -v "$INSTALL_DIR/share/discord-bsd/discord.desktop" "$INSTALL_DIR/share/applications/"
    mv -v "$INSTALL_DIR/share/discord-bsd/discord" "$INSTALL_DIR/bin"

    sed -i '' "s|Exec=auto1|Exec=$HOME/.local/bin/discord|" "$INSTALL_DIR/share/applications/discord.desktop"
    sed -i '' "s|Icon=auto2|Icon=$HOME/.local/share/discord-bsd/discord.png|" "$INSTALL_DIR/share/applications/discord.desktop"

    cd "$INSTALL_DIR/share/discord-bsd" || exit
    npm install

    printf "Discord 0.0.25 installed!\n"
    printf "\nSetting executable perms!"
    chmod +x "$INSTALL_DIR/bin/discord"
    printf "\nDiscord is ready to launch and use! Thank you for using this port! \n"
}

printf "Launching installer...\n" && install
