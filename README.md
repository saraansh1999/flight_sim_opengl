Graphics Assignment 2

This is a basic flight simulator kind of game in which the player controls a Fighter Jet and has checkpoints as missions to complete.

To Run:
- sudo apt-get update
- sudo apt-get install libglfw3
- sudo apt-get install libglfw3-dev
- sudo apt-get install libglew
- sudo apt-get install libglew-dev
- sudo apt-get install libglm-dev
- Go to 'graphics-boilerplate-master/build'.
- Run 'cmake ..'.
- Run 'make all'.
- Run './graphics_asgn1'.

How To Play:
1. Control the altitude of the plane using Spacebar for 'UP' and 'LeftAlt' for 'DOWN'.
2. Control the speed of the plane using 'W'.
3. Control the yaw of the plane using 'A' and 'D'.
4. Control the roll of the plane using 'Q' and 'E'.
5. Use 'LeftMouseButton' to shoot missiles.
6. Use 'RightMouseButton' to drop bombs.
7. Change views according to the following keys:
                '1': Follow-cam view.
                '2': Top view.
                '3': Tower view.
                '4': FPS view.
                '5': Helicopter view.
8. In the FPS view a Crosshair appears and the player can use the 'Scroll' to zoom in(to aid in shooting) or out.
9. In the Helicopter view the player can use the mouse to look in any direction desirable and can also use the        'Scroll' to move the Helicopter forwards or backwards in the current facing direction.
10. The dashboard features the following:
                 1. Top-left corner: Compass with Red Stick pointing North and Black Stick pointing in the next   Checkpoint's direction.
                 2. Top-right corner: Fuel indicator.
                 3. Bottom-right corner: Altitude indicator.
                 4. Bottom-left corner: Speed indicator.
                 5. Top-middle: Lives left.
                 6. Bottom-middle: Points collected.
11. Points distribution:
                 '+10 points': Eliminating a ground station.
                 '+20 points and +1 life': Eliminating an enemy parachute
                 '+5': Passing through a smoke ring.
12. Ways to loose lives and loose the game:
                 1. Fly below sea level and drown.(immediately GAME OVER!)
                 2. Fly above a Volcano and burn.(immediately GAME OVER!)
                 3. Fly into a ground station and crash.(immediately GAME OVER!)
                 4. Loose a life by getting hit by enemy missiles and loose once they're over.
13. Fuel-ups can be collected to stay in the game for a longer period of time.
                    


