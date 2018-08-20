## Fusion
Fun mind teasing puzzle game

## How to install? [Ubuntu]

* Install dependencies
    * `sudo apt-get install libx11-dev`
* Open terminal in this directory
    * `mkdir build && cd build`
    * `cmake ..`
    * `make` to compile code [creates an executable **fusion**]
    * `./fusion` to start playing

## How to play?
* The game board is circular
* An element (from periodic table if you remember) pops up in the center
* It can be placed anywhere on the circular board
* As one element is placed a new element appears again in the center
* Sometimes a special element `+` pops up
    * Once placed in circle, It has the power to fuse other elements
    * But for fusion to occur elements on either side of `+` should be mirror images
    * For Ex. If board has `H` `+` `H`, then both `H` fuse to give `He`
    * More than one pair of elements can be fused at a time
    * For ex: `Li` `H` `+` `H` `Li` gives `Be`
    * Notice the number of elements decrease when fusion occurs
* Every time a fusion occurs you get a point
* However, not more than a **12** elements can be present on the board
* So your job is to strategically place the elements and `+` to keep the fusion going
* So place, fuse and repeat to score the highest

## Credits
Inspired by Atomas game in Google Play Store
