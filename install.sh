#!/bin/sh
printf "Discord 0.0.25\n"
printf "Discord desktop client based on Discord Web for FreeBSD\n"
printf "\n Do not run this in root! Or else this script breaks and will replicate your current chdir! It will not install under root!\n"
install() {
	printf "Installing...\n"
	mkdir -p ~/.local/share/discord-bsd
	mkdir -p ~/.local/bin
	mkdir -p ~/.local/share/applications
	cp -rv ./* ~/.local/share/discord-bsd
	mv -v ~/.local/share/discord-bsd/discord.desktop ~/.local/share/applications/
	mv -v ~/.local/share/discord-bsd/discord ~/.local/bin
	sed -i '' "s/Exec=auto1/Exec=\/home\/$USER\/.local\/bin\/discord/" ~/.local/share/applications/discord.desktop
	sed -i '' "s/Icon=auto2/Icon=\/home\/$USER\/.local\/share\/discord-bsd\/discord.png/" ~/.local/share/applications/discord.desktop
	cd /home/$USER/.local/share/discord-bsd
	npm install
	printf "Discord 0.0.25 installed!\n"
        printf "\n Setting executable perms!"
	chmod +x ~/.local/bin/discord
        printf "\n Discord is ready to launch and use! Thank you for using this port!"
}
printf "Launching installer...\n" && install
