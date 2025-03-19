# Docker QNX Build Environment for macOS

This repo contains the files you need to create a Docker-based build environment to build QNX projects on a macOS host system. It also contains `my-project`, a sample QNX "Hello World" project created in Visual Studio Code with the QNX Toolkit extension -- it can be used to test your Docker environment after you prepare it.

You can read more about this environment on the QNX Developer Blog here: https://devblog.qnx.com/how-to-use-macos-to-develop-for-qnx-8-0/

# Prerequisites

You'll need the following before trying to use the Docker environment:

1. Get a license for QNX 8.0 (free for non-commercial use), receive it, and deploy it to yourself: qnx.com/getqnx.
    - You'll know you're done when you see the license deployed to yourself in the QNX Licence Manager.
2. Install Docker Desktop on your Mac and confirm the included Hello World example works: https://www.docker.com/get-started/.
3. Install Visual Studio Code: https://code.visualstudio.com/download.
4. Use VS Code to install the QNX Toolkit extension (instructions here).
5. Install QNX Software Center for macOS Hosts from the myQNX website (direct link).
6. Log in to QNX Software Center (QSC) with your myQNX account and activate your license on the License page.
7. Follow the next sections to get the host tools for Linux and to configure VS Code.

## Get QNX Host Tools for Linux
The Docker build environment needs access to the QNX Software Development Platform (SDP) and the Linux Host Tools.

1. On the main (home) page of QNX Software Center (QSC), click Add Installation...
2. Choose QNX Software Development Platform 8.0 [com.qnx.qnx800] and click Next at the bottom.
3. Follow the rest of the wizard, making sure to install the SDP to the recommended /Users/username/qnx800 directory.
4. When the SDP installation is complete, we can add the Linux Host Tools:
    - Go to the Manage Installation section (left menu icon with a checkmark)
    - Click the Available tab
    - At the top, search for Host Tools and find QNX SDP Host Tools (Linux 64-bit) in the list
    - Click Next at the bottom and follow the wizard to install the host tools

## Configure VS Code
There are a few settings to configure in the QNX Toolkit extension for VS Code.

1. In VSCode, open View → Extensions.
2. Beside QNX Toolkit on the left tab, click the settings cog and choose Settings.
3. Set the SDP path to the location you set in step 2 (the default should be `/Users/<username>/qnx800`).
4. Create the folder `~/qnxprojects` on your Mac (i.e. in your home directory).
5. In VS Code, set the Default Project Root to `${userHome}/qnxprojects`.

# Create the Docker Container

1. Clone this repo or download the contents as a zip. Browse to the `docker-mac` directory within in.
2. Run `docker-build-qnx-image.sh` – this will create an image in the Images tab of Docker Desktop. You should see a new image created called `qnx800`.
3. Run `docker-create-container.sh` – this will create a container from the image, and will mount your Mac's HOME directory in the container to help with building. You can see the container in the Containers list of Docker Desktop.
4. Make sure the qnx-build container is running in Docker Desktop.

# Build a Project

1. Open the `my-project` directory included in this repo using VS Code.
2. Right click on the files in the project and choose QNX > Build Active Project.

A binary should be created in the root of the project. You can now transfer this binary to a target to run it.

Open `Makefile` and `build.sh` for details about how we're executing the build. In these files you can customize how your project is being built, such as adjusting the target architecture.