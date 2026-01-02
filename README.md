# Dining Philosophers
A simple simulation where N "philosopher" processes and N "chopstick" semaphores are created.
Each philosopher infinitely alternates between "eating" and "thinking" for a short time.
Philosopher i needs chopsticks i (left) and (i+1)%N (right) to start eating. When a philosopher stops eating, it puts down both chopsticks.
The simulation ends automatically after a certain time (to change that time, edit the value of SIMULATION_TIME in src/main).

For more information, check https://en.wikipedia.org/wiki/Dining_philosophers_problem

## Types of philosophers
Right now, there are few types of philosopher process, but my goal is to implement several different solutions that avoid
problems such as deadlock and starvation.

* **basic (pathname: "bin/philosopher")**  
  Tries to pick up the left chopstick. After it acquires the left chopstick, it tried to pick up the right chopstick. Never puts down
  a chopstick unless it just finished eating. Every philopher starts in "eating" mode. This solution is prone to deadlock and starvation
  (imagine what would happen if every philosopher immediately picked up the chopstick to their left)

* **asymmetric (pathname: "bin/philosopher_asymm)"**  
  Same as basic, but instead of always picking up the left chopstick first, philosophers with an even index will start with the left,
  but philophers with an odd index will start from the right. This solution avoid deadlock.

## Compiling and executing
To compile simply run "make all" in the main directory.

To run, open you terminal emulator of choice and navigate to the main directory of the program.
From there, run the following command:

`./bin/dining_philosophers N path_name`

replacing N with the number of philosopher processes you want to create and path_name with the path to the philosopher executable, wether 
it's one of those provided with the program (you can see their paths in the previous section of this file). Leaving path_name blank will 
make the program look for a file named bin/philosopher which is the default executable provided with the program). 
