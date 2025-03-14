<a id="readme-top"></a>

# Windows & Office Activation

A simple tool for permanently activating **Windows 10/11** and **Microsoft Office** on Windows 8 and later.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Windows Activation

Permanently activates Windows 10/11 using your system's hardware ID.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Features

-   Does not modify or store any files on your system
-   Provides permanent activation tied to your hardware
-   Activation can be linked to a Microsoft account without issues
-   Once activated, the license remains stored on Microsoft's servers and cannot be removed from your system
-   Automatic reactivation after reinstalling Windows, as long as the hardware remains unchanged
-   Requires an active internet connection

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Supported Editions

| Edition Type       | Variants                                                                                                                                   |
| ------------------ | ------------------------------------------------------------------------------------------------------------------------------------------ |
| **Education**      | Education, Education N                                                                                                                     |
| **Enterprise**     | Enterprise, Enterprise N, Enterprise LTSB 2015, Enterprise LTSB 2016, Enterprise LTSC 2019, Enterprise N LTSB 2015, Enterprise N LTSB 2016 |
| **Home**           | Home, Home N, Home China, Home Single Language                                                                                             |
| **IoT Enterprise** | IoT Enterprise, IoT Enterprise Subscription, IoT Enterprise LTSC 2021, IoT Enterprise LTSC 2024, IoT Enterprise LTSC Subscription 2024     |
| **Pro**            | Pro, Pro N, Pro Education, Pro Education N, Pro for Workstations, Pro N for Workstations                                                   |
| **S**              | S, S N                                                                                                                                     |
| **SE**             | SE, SE N                                                                                                                                   |
| **Team**           | Team                                                                                                                                       |

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### How does it work?

The activator installs a generic Retail, OEM, or MAK key, then patches gatherosstate.exe (downloaded from the MS ADK) via GamersOsState to generate a genuine ticket to create a valid digital license.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Office Activation

Activates most Office versions on Windows 8 and later, including Server editions, except Office 2010 and UWP apps.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Features

-   Works offline and stays activated permanently
-   Survives Office repairs, updates, and Windows upgrades without reactivation
-   Server side Office 365 features like 1TB OneDrive storage are not available, but all other features and 5GB of free storage remain
-   Does not modify system files, just installs a custom open source SPPC.dll for activation

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Credits

-   GamersOsState by asdcorp - [GitHub](https://github.com/asdcorp/GamersOsState/)
-   ohook by asdcorp - [GitHub](https://github.com/asdcorp/ohook/)

<p align="right">(<a href="#readme-top">back to top</a>)</p>
