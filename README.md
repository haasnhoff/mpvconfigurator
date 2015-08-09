![http://mpv.io/](https://raw.githubusercontent.com/haasnhoff/mpvconfigurator/master/src/images/icon.png)

## mpv Configurator

--------------


* [Overview](#overview)
* [Downloads](#downloads)
* [FAQ:](#faq)
* [Bug reports](#bug-reports)
* [License](#license)

## Overview

**mpv Configurator** is a simple GUI that creates mpv.conf files that the media player mpv can use. 

Currently, mpv Configurator is in beta. This means it's not very optimized and might screw up on different operating systems. 

## Downloads

Pre-compiled binaries for mpv Configurator can be found in the [releases section](https://github.com/haasnhoff/mpvconfigurator/releases)

Currently only betas are available. The configurator will be slated for 1.0 release when it has been properly tested, updating manuals feature and tooltips are fixed.

**Note for GNU/Linux users:**

While the GNU/Linux version of mpv Configurator worked out of the box on a Ubuntu 14.04 live CD without modifications or required downloads, GNU/Linux has very special deployment methods thus it might not work OOB in all distributions. If it gives off a segfault or require dependencies, you'll need to download the required libraries, whatever they might be. Try ldd and see what is missing.

I might statically build the GNU/Linux binary later on for easier deployment, but if you keep having problems with it, please download Qt5 and compile it yourself. I am painfully aware of how stupid this is, but deploying shared library Qt applications on GNU/Linux are a pain in the ass, sorry.

## FAQ:

**Are you affiliated with the mpv guys?**

No.

**Some tooltips aren't working as inteded. Comboboxes should have tooltips.**

This is known.

**Your code is absolutely horrible, please commit suicide.**

This is my second "major" C++ project, please bear with me. 

**Where is the documentation?**

Under construction.

## Bug reports

Please use the issue-tracker provided by GitHub to send me bug
reports or feature requests.

## License 

[GPLv2](https://github.com/haasnhoff/mpvconfigurator/blob/master/LICENSE)