# AGS Game Project
This is the Game project for the AGS NZOI group.

## Requirements

This project uses:
* glew
* glfw
* glm
* opengl
* (later) Box2D

These will be needed to develop this project, they can be found using Google.
If you need help for the dependencies just ask.

## Box2D
Read the manual: http://box2d.org/manual.pdf

## Contributing

The main branch should be kept in a working state. This means that all commits should be made to a separate branch
and committed later. The branch should only be merged when at least the header files and basic functionality is done.
You can add to **your** header files after to add new functionality but try not to remove or change methods without consulting people who may be using them.

If you want a feature create an issue, someone whose job is most close to the requested feature or whose code it involved will assign themselves or get assigned to it by the project manager.

Large additions that affect the whole project should be added using a pull request so others can have a look, otherwise simply merge using the command line or whatever other tool floats your boat, for more info click [here](http://gitref.org/branching/).

## Jobs & Responsibilities

Name | Jobs
-----|------
James | Renderer & Box2D
Mitchell | User Interface Design
Andrew | General Logic & Build Systems
Byung Hoon | Project Manager
Ashutosh | Graphics and Sever
Kevin | Gameplay and Graphics
Hogan | Quality Assurance

Add these people to the assignee of the issue if you think they would be best suited. Else leave it blank and someone will
assign themselves if they have time.

## Discussion

We are using [Slack](https://agsprogramming.slack.com) for discussion but there is a meeting every Friday.

## Building

### Normally

#### Unix
- Run `./scripts/build.sh`  
- To run, run `./scripts/run.sh`  

#### MinGW
NOTE: You need TDM-GCC-64  
- Run `scripts\build_mingw.bat` through double clicking on it or through the command line  
- To run, run `scripts\run.bat` through double clicking on it or through the command line  

### Eclipse

#### Unix

- Run `./scripts/eclipse.sh`  
- Import the generated project in Eclipse  
- Run 'client'  

#### MinGW

- Run `scripts\eclipse_mingw.bat` through double clicking on it or through the command line  
- Import the generated project in Eclipse  
- Run 'client'  
