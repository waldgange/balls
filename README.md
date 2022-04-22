# balls
Balls is a project for simulation of 2D balls collisions. There are two applications in this project:
* **ballsapp**
![](/static/balls-screen.png?raw=true)

It is a gui application, that demonstrates collisions of balls. You can add balls and switch algorithms, used to process scene.
* **benchmark**

It is a CLI application, that measures time of processing specified number of frames with specified algorithms.

**Usage:**
```
benchmark <pre_phase_type> <broad_phase_type> <narrow_phase_type> <number_of_balls> <number_of_frames>
```
**Example:**
```
$ ./benchmark s qw s 1000 1
adding 1000 balls...
processing 1 frames...
1 frames processed in 4.5262 seconds
$ ./benchmark s ss s 1000 1
adding 1000 balls...
processing 1 frames...
1 frames processed in 0.0961435 seconds
```

**Qwick start:**
```
git clone https://github.com/waldgange/balls.git
mkdir build && cd build
cmake ../balls
make
```
