const { app, BrowserWindow, Tray, Menu, shell, nativeImage } = require('electron');
const path = require('path');

let mainWindow = null;
let bootWindow = null;
let appIcon = null;
const iconPath = path.join(__dirname, 'discord.png');
const bootVideoPath = path.join(__dirname, 'assets', 'boot.mp4');

function createBootWindow() {
    bootWindow = new BrowserWindow({
        width: 600,
        height: 400,
        frame: false,
        alwaysOnTop: true,
        webPreferences: {
            nodeIntegration: true
        }
    });

    bootWindow.setMenuBarVisibility(false);
    bootWindow.setFullScreenable(false);

    bootWindow.loadFile('boot.html');
    bootWindow.webContents.on('did-finish-load', () => {
        bootWindow.show();
        bootWindow.webContents.executeJavaScript(`
            const video = document.createElement('video');
            video.src = '${bootVideoPath}';
            video.autoplay = true;
            video.muted = true;
            video.loop = false;
            video.style.width = '100%';
            video.style.height = '100%';
            video.style.objectFit = 'cover';
            document.body.style.margin = '0';
            document.body.style.overflow = 'hidden';
            document.body.appendChild(video);

            video.addEventListener('ended', function() {
                window.api.send('boot-video-ended');
            });
        `);
    });

    return bootWindow;
}

function createMainWindow() {
    mainWindow = new BrowserWindow({
        width: 1280,
        height: 720,
        show: false,
        webPreferences: {
            nodeIntegration: true
        },
        icon: iconPath,
        title: 'Discord',
        autoHideMenuBar: true
    });

    mainWindow.loadURL('https://discord.com/app');
    bootWindow = createBootWindow();
    mainWindow.once('ready-to-show', () => {
        mainWindow.show();
        bootWindow.close();
        appIcon.setContextMenu(Menu.buildFromTemplate([
            {
                label: 'Open Discord',
                click: function () {
                    mainWindow.show();
                }
            },
            {
                label: 'Quit',
                click: function () {
                    app.quit();
                }
            }
        ]));
    });

    return mainWindow;
}

app.on('ready', () => {
    createMainWindow();
    appIcon = new Tray(nativeImage.createFromPath(iconPath));
    appIcon.setToolTip('Discord');
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});
