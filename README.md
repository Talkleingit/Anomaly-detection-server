
<!-- Title -->
## Anomlay Detection Server in C++



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
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#Features-and-Functionality">Features and Functionality</a></li>
  
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
This project was written entirely in C++ and provides an anomaly detection server that uses 2 different algorithms in order to identify anomalies in given csv files.





### Built With
1. C++





<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running, follow these steps.


### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/Talkleingit/Anomaly-detection-server.git
   ```
2. 
   

<!-- Features-and-Functionality -->
## Features and Functionality
 The server expects to receive from the user a CSV file with all features the user would like to test. The user also specifies which algorithm he wishes the server to use
 (Regression or Hybrid). Then the server using a CLI component designed with the command behavioural design pattern executes the given commands by the user. The first command
 is responsible for collecting 2 csv files from the user, one for the learning phase and another for the matter of detection. The second command is responsible for setting the 
 detection algorithm, Simple (Linear regression) or Hybrid (Min encapsulating circle). The third command is responsible for the detection, meaning it runs an analysis on the 
 detection file uploaded by the user and verifies if all attributes correlate to the findings of the algorithm in the learnNormal stage. The fourth command displays 
 the algorithms results. The fifth command checks for FP and TP rates. The sixth command is responsible for stopping the connection.



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/github_username
