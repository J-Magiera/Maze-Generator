<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
***
***
***
*** To avoid retyping too much info. Do a search and replace for the following:
*** github_username, repo_name, twitter_handle, email, project_title, project_description
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->






<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/J-Magiera/Maze-Generator">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Maze Generator</h3>

  <p align="center">
    Implementation of maze generation algorithm on STM32F429xx family MC with 240x320 LCD screen
    <br />
    <a href="https://github.com/J-Magiera/Maze-Generator"><strong>Explore the docs »</strong></a>
    <br />
    <br />
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This is a maze generator project, built for STM32F429xx MC family.
It was built by [Jan Magiera](https://github.com/J-Magiera) and [Paweł Waśniowski](https://github.com/wisnia1998).
To generate maze, project utilizes depth-first search, also know as recursive backtracker algorithm.
Resources are managed by FreeRTOS. External input device communicates via UART interface with built in ring buffer.



### Built With

* [FreeRTOS](https://www.freertos.org/)




<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

In order to use this project, you need:
* Keil uVision or other ARM IDE
  ```sh
  https://www.keil.com/download/product/
  ```
* STM32F4xx family board

* (Optional) FRDM KL05Z board


### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/J-Magiera/Maze-Generator.git
   ```
2. Open ARM IDE

3. Create "New project"

4. Select Device for Target
   ```sh
   STMicroelectronics ->
	STM32F429xx family
   ```
5. Include appropriate files

6. Include all source and header files from "Display" folder

7. Compile, build and load on flash



<!-- USAGE EXAMPLES -->
## Usage

Empty as of today




<!-- CONTACT -->
## Contact

Jan Magiera: (Jan.Magiera@Protonmail.com)
Paweł Waśniowski: (W.Pawel1998@gmail.com)

Project Link: [https://github.com/J-Magiera/Maze-Generator](https://github.com/J-Magiera/Maze-Generator)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements

* [FreeRTOS](https://www.freertos.org/)
* [WSN AGH](http://www.wsn.agh.edu.pl/)
* []()






