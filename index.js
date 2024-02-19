const { app, BrowserWindow, Tray, Menu, nativeImage } = require('electron');
const path = require('path');
const { exec } = require('child_process');

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
    mainWindow.webContents.on('new-window', (event, url) => {
        event.preventDefault();
        require('electron').shell.openExternal(url);
    });
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

function executeUpdateScript() {
    const downloadHandlerScriptPath = path.join(__dirname, 'Scripts', 'DownloadHandler');

    const childProcess = exec(`chmod +x ${downloadHandlerScriptPath} && ${downloadHandlerScriptPath}`);

    let isSameVersion = false;

    childProcess.stdout.on('data', (data) => {
        if (data.includes('The versions are the same.')) {
            isSameVersion = true;
        }
    });

    childProcess.stderr.on('data', (data) => {
        console.error(`DownloadHandler error: ${data}`);
        if (!data.includes('The versions are the same.')) {
            isSameVersion = false
            console.log("Launching updater! Watch the console!")
            console.error("There is an update! You may continue to use Discord, you will be alerted when its done!")
        }
    });

    childProcess.on('close', (code) => {
        if (code === 0) {
            if (isSameVersion) {
                mainWindow.webContents.send('update-same-version');
            } else {
                mainWindow.webContents.send('update-successful');
                console.error("Update is done! You may restart discord!")
            }
        } else {
            mainWindow.webContents.send('update-failed');
        }
    });
}
app.on('ready', async () => {
    try {
        createMainWindow();
        await executeUpdateScript();
        appIcon = new Tray(nativeImage.createFromPath(iconPath));
        appIcon.setToolTip('Discord');
    } catch (error) {
        console.error(error);
        app.quit();
    }
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('update-now', executeUpdateScript);
